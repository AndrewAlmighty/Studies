#include "controller.hpp"
#include "guicontroller.hpp"

#include <chrono>
#include <thread>
#include <utility>
#include <string.h>
#include <iostream>

Controller::Controller(GuiController *ptr)
{
    m_GuiPtr = ptr;
    loop();
}

Controller::~Controller()
{
    m_LoopOn = false;
    Message msg;
    msg.type = Disconnect;
    std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, msg.text);
    for (const auto& dev : m_devicesList)
    {
        if (dev.first.first == m_HostIp && dev.first.second == m_port)
            continue;

        sendMessage(&m_Socket, &msg, m_port, dev.first.first.c_str(), &dev.first.second);
    }
    shutdownSocket(&m_Socket);
}

bool Controller::connectTo(const std::string &ip, const unsigned &port)
{
    Message msg;
    msg.type = ConnectionRequest;
    msg.sender_port = m_port;
    std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, msg.text);
    sendConnectionRequest(&m_Socket, ip.c_str(), &port, &msg);
    msg.type = EmptyMessage;

    auto start_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time);

    m_holdLoop = true;
    while (duration.count() < 5)
    {
        duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time);
        checkMessageBox(&m_Socket, &msg);
        if (msg.type != EmptyMessage)
            std::cerr << "MSGTYPE:" << msg.type << std::endl;

        if (msg.type == ConnectionAccepted)
        {
            m_holdLoop = false;
            std::cerr << "Mamy polaczenie\n";
            return true;
        }

        else
            handleMsg(msg);
    }

    m_holdLoop = false;
    return false;
}

bool Controller::setNetworking(const std::string &ip, const unsigned &port)
{
    if (createAndBindSocket(&m_Socket, &port))
    {
        m_IsReady = true;
        m_HostIp = ip;
        m_port = port;
        m_devicesList.insert(std::pair<addr, unsigned>(addr(ip, port), 0));
        return true;
    }

    else
    {
        m_IsReady = false;
        m_HostIp.clear();
        return false;
    }
}

bool Controller::isReady() const
{
    return m_IsReady;
}

void Controller::enterToCriticalSection()
{
    std::thread thread([&]() {
        std::cerr << "rozmiar listy urzadzen: " << m_devicesList.size() << std::endl;
        auto start_time = std::chrono::system_clock::now();
        auto duration = start_time.time_since_epoch();

        Message msg;
        msg.type = AskToEnterCritical;
        msg.timestamp = duration.count();
        m_curTimestamp = duration.count();
        std::cerr << "timestamp:" << m_curTimestamp << std::endl;
        m_GuiPtr -> setStatus(QString::fromStdString("current timestamp:" + std::to_string(m_curTimestamp)));
        std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, msg.text);
        for (const auto &dev : m_devicesList)
        {
            if (dev.first.first == m_HostIp && dev.first.second == m_port)
                continue;

            std::cerr << "Wysylamy prosbe o wejscie do sekcji do:" << dev.first.first << ":" << dev.first.second << std::endl;
            sendMessage(&m_Socket, &msg, m_port, dev.first.first.c_str(), &dev.first.second);
        }

        Message newMsg;
        newMsg.type = EmptyMessage;
        bool can_process = false;
        while (m_curTimestamp != 0)
        {
            m_holdLoop = true;
            checkMessageBox(&m_Socket, &newMsg);
            handleMsg(newMsg);
            can_process = true;

            for (const auto &dev : m_devicesList)
            {
                if (dev.first.first == m_HostIp && dev.first.second == m_port)
                    continue;

                if (dev.second < m_curTimestamp && dev.second != 0)
                {
                    std::cerr << "Mamy proces z mniejszym timestampem:" << dev.first.first << ":" << dev.first.second << std::endl;
                    can_process = false;
                    break;
                }
            }

            if (can_process)
                break;

            newMsg.type = EmptyMessage;
        }

        if (m_curTimestamp != 0)
        {
        m_inCritical = true;
        m_curTimestamp = 0;
        m_GuiPtr -> setInCritical(true);
        std::cerr << "Jestesmy w sekcji krytycznej!\n";
        }
        m_holdLoop = false;
    });
    thread.detach();
}

void Controller::leaveCriticalSection()
{
    m_inCritical = false;
    m_curTimestamp = 0;
    std::cerr << "Opuszczamy sekcje krytyczna!\n";
    m_GuiPtr -> setInCritical(false);

    for (auto &dev : m_devicesList)
    {
        if (dev.first.first == m_HostIp && dev.first.second == m_port)
            continue;

        Message msg;
        msg.type = LeaveCritical;
        std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, msg.text);
        sendMessage(&m_Socket, &msg, m_port, dev.first.first.c_str(), &dev.first.second);
    }
}

void Controller::reset()
{
    if (m_Socket != 0)
        shutdownSocket(&m_Socket);

    m_IsReady = false;
    m_Socket = 0;
    m_HostIp = "";
    m_port = 0;
}

void Controller::loop()
{
    std::thread thread([&]()
    {
        Message msg;

        auto start_time = std::chrono::steady_clock::now();
        while (m_LoopOn)
        {
            if (!m_IsReady || m_holdLoop)
                continue;

            if (m_curTimestamp == 0)
            {
                checkMessageBox(&m_Socket, &msg);

                if (msg.type != EmptyMessage)
                    handleMsg(msg);
            }


            auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time);

            if (duration.count() > 15)
            {
                checkConnection();
                start_time = std::chrono::steady_clock::now();
            }
        }
    });

    thread.detach();
}

void Controller::handleMsg(Message &msg)
{
    switch (msg.type)
    {
    case EmptyMessage:
        break;

    case ConnectionRequest:
        handleConnectionRequest(std::string(msg.text),msg.sender_port);
        m_GuiPtr->addDevice(msg.text, msg.sender_port);
        break;

    case ConnectionAccepted:
        break;

    case NewDevice:
        std::cerr << "nowe urzadzenie! Addr:" << msg.text << ":" << msg.timestamp << std::endl;
        m_devicesList.insert(std::pair<addr, unsigned>(addr(msg.text, static_cast<unsigned>(msg.timestamp)), 0));
        m_GuiPtr -> addDevice(msg.text, static_cast<unsigned>(msg.timestamp));
        break;

    case Disconnect:
        std::cerr << "Usunieto urzadzenie! Addr:" << msg.text << ":" << msg.sender_port << std::endl;
        m_devicesList.erase(addr(msg.text, msg.sender_port));
        m_GuiPtr -> removeDevice(msg.text, msg.sender_port);
        break;

    case CheckConnection:
    {
        Message tmpMsg;
        tmpMsg.type = ConnectionAck;
        std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, msg.text);
        sendMessage(&m_Socket, &tmpMsg, m_port, msg.text, &msg.sender_port);
    }
        break;

    case ConnectionAck:
        break;

    case AskToEnterCritical:
    {
        std::cerr << "Prosba o wejscie do sekcji krytycznej!\n";

        for (auto &dev : m_devicesList)
        {
            if (dev.first.first == msg.text && dev.first.second == msg.sender_port)
            {
                dev.second = msg.timestamp;
                m_GuiPtr->changeTimestamp(dev.first.first, dev.first.second, msg.timestamp);
                break;
            }
        }

        if (!m_inCritical && m_curTimestamp == 0)
        {
            std::cerr << "Prosba o wejscie do sekcji krytycznej - opcja 1\n";
            Message tmpMsg;
            tmpMsg.type = AnswerForAsk;
            std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, tmpMsg.text);
            std::cerr << "Moje ip:" << msg.text << std::endl;
            sendMessage(&m_Socket, &tmpMsg, m_port, msg.text, &msg.sender_port);
        }

        else
        {
            std::cerr << "Prosba o wejscie do sekcji krytycznej - opcja 2\n";
            m_holdLoop = true;
            {
                Message tmpMsg;
                tmpMsg.type = EmptyMessage;
                bool can_process = false;
                while (m_curTimestamp != 0)
                {
                    checkMessageBox(&m_Socket, &tmpMsg);
                    handleMsg(tmpMsg);
                    can_process = true;

                    for (const auto &dev : m_devicesList)
                    {
                        if (dev.first.first == m_HostIp && dev.first.second == m_port)
                            continue;

                        if (dev.second < m_curTimestamp && dev.second != 0)
                        {
                            std::cerr << "Mamy proces z mniejszym timestampem:" << dev.first.first << ":" << dev.first.second << std::endl;
                            can_process = false;
                            break;
                        }
                    }

                    if (can_process)
                    {
                        m_curTimestamp = 0;
                        m_inCritical = true;
                        m_GuiPtr -> setInCritical(true);
                        break;
                    }

                    tmpMsg.type = EmptyMessage;
                }
            }

            std::cerr << "Krok 1 - curTimestamp == 0\n";
            {
                Message tmpMsg;
                tmpMsg.type = EmptyMessage;
                while (m_inCritical)
                {
                    checkMessageBox(&m_Socket, &tmpMsg);
                    handleMsg(tmpMsg);
                }
            }
            std::cerr << "Krok 2 - m_inCritical == false\n";

            for (auto &dev : m_devicesList)
            {
                if (dev.second == 0 || (dev.first.first == m_HostIp && dev.first.second == m_port))
                    continue;

                dev.second = 0;
                m_GuiPtr->changeTimestamp(dev.first.first, dev.first.second, 0);
                Message tmpMsg;
                tmpMsg.type = AnswerForAsk;
                std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, tmpMsg.text);
                sendMessage(&m_Socket, &tmpMsg, m_port, dev.first.first.c_str(), &dev.first.second);
            }
            m_holdLoop = false;
        }
    }

        break;

    case AnswerForAsk:
        for (auto &dev : m_devicesList)
        {
            if (dev.first.first == m_HostIp && dev.first.second == m_port)
                continue;

            if (dev.first.first == msg.text && msg.sender_port == dev.first.second && dev.second != 0)
            {
                dev.second = 0;
                std::cerr << "Zerujemy " << dev.first.first << ":" << dev.first.second << std::endl;
                m_GuiPtr -> changeTimestamp(dev.first.first, dev.first.second, 0);
                break;
            }
        }
        break;

    case LeaveCritical:
        for (auto &dev : m_devicesList)
        {
            if (dev.first.first == m_HostIp && dev.first.second == m_port)
                continue;

            if (dev.first.first == msg.text && msg.sender_port == dev.first.second && dev.second != 0)
            {
                dev.second = 0;
                m_GuiPtr->changeTimestamp(dev.first.first, dev.first.second, 0);
                break;
            }
        }
        break;
    }

    msg.type = EmptyMessage;
}

void Controller::handleConnectionRequest(std::string ip, unsigned port)
{
    std::cerr << "rozmiar listy urzadzen:" << m_devicesList.size() << std::endl;
    std::cerr << "ip nowego urzadzenia:" << ip << ":" << port << std::endl;
    for (const auto &dev : m_devicesList)
    {
        Message msg;
        msg.type = NewDevice;
        std::cerr << "Wysylamy info do prosbodawcy o:" << dev.first.first << ":" << dev.first.second << std::endl;
        msg.timestamp = dev.first.second;
        std::copy(dev.first.first.c_str(), dev.first.first.c_str() + MSG_MAX_LEN, msg.text);
        sendMessage(&m_Socket, &msg, m_port, ip.c_str(), &port);
    }

    for (const auto &dev : m_devicesList)
    {
        Message msg;
        msg.timestamp = port;
        std::copy(ip.c_str(), ip.c_str() + MSG_MAX_LEN, msg.text);
        std::cerr  << dev.first.first << ":" << dev.first.second << std::endl;
        std::cerr << "moje:" << m_HostIp << ":" << m_port << std::endl;
        if (dev.first.first != m_HostIp || dev.first.second != m_port)
        {
            std::cerr << "Wysylamy info o nowym device do:" << dev.first.first << ":" << dev.first.second << std::endl;
            sendMessage(&m_Socket, &msg, m_port, dev.first.first.c_str(), &dev.first.second);
        }
    }
    m_devicesList.insert(std::pair<addr, unsigned>(addr(ip, port), 0));
    Message msg;
    msg.type = ConnectionAccepted;
    sendMessage(&m_Socket, &msg, m_port, ip.c_str(), &port);
    std::cerr << "Nowe urządzenie\n";
}

void Controller::checkConnection()
{
    std::unordered_map<addr, bool, addrhash> checklist;

    for (const auto &dev : m_devicesList)
        checklist.insert(std::pair<addr, bool>(addr(dev.first.first, dev.first.second), false));

    {
        Message msg;
        msg.type = CheckConnection;
        for (auto &dev : checklist)
        {
            if (dev.first.first == m_HostIp && dev.first.second == m_port)
                continue;

            std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, msg.text);
            sendMessage(&m_Socket, &msg, m_port, dev.first.first.c_str(), &dev.first.second);
        }
    }

    auto start_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time);
    {
        Message msg;
        msg.type = EmptyMessage;
        bool do_break = false;
        while (duration.count() < 5)
        {
            checkMessageBox(&m_Socket, &msg);
            if (msg.type != ConnectionAck)
                handleMsg(msg);

            else
            {
                for (auto &dev : checklist)
                {
                    if (dev.first.first == msg.text && dev.first.second == msg.sender_port)
                    {
                        dev.second = true;
                        break;
                    }
                }

                do_break = true;
                for (auto &dev : checklist)
                {
                    if (dev.second == false)
                        do_break = false;
                }
            }

            if (do_break == true)
                break;
        }
    }

    for (const auto &dev : checklist)
    {
        if (dev.second == false)
        {
            m_devicesList.erase(addr(dev.first.first, dev.first.second));
            m_GuiPtr -> removeDevice(dev.first.first.c_str(), dev.first.second);

        }
    }
}
