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

    while (duration.count() < 5)
    {
        duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time);
        checkMessageBox(&m_Socket, &msg);
        if (msg.type != EmptyMessage)
            std::cerr << "MSGTYPE:" << msg.type << std::endl;

        if (msg.type == ConnectionAccepted)
        {
            std::cerr << "Mamy polaczenie\n";
            return true;
        }

        else
            handleMsg(msg);
    }

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
        std::unordered_map<int, bool> accept_list;
        int i = 0;
        for (const auto& dev : m_devicesList)
        {
            if (dev.first.first == m_HostIp && dev.first.second == m_port)
                continue;

            accept_list.insert(std::pair<int, bool>(i, false));
            i++;
        }

        std::cerr << "rozmiar listy urzadzen: " << m_devicesList.size() << std::endl;
        auto start_time = std::chrono::system_clock::now();
        auto duration = start_time.time_since_epoch();

        Message msg;
        msg.type = AskToEnterCritical;
        msg.timestamp = duration.count();
        m_curTimestamp = duration.count();
        std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, msg.text);
        for (const auto &dev : m_devicesList)
        {
            if (dev.first.first == m_HostIp && dev.first.second == m_port)
                continue;

            sendMessage(&m_Socket, &msg, m_port, dev.first.first.c_str(), &dev.first.second);
        }

        std::cerr << "i:" << i << std::endl;
        Message newMsg;
        newMsg.type = EmptyMessage;
        while (i > 0 && m_curTimestamp != 0)
        {
            checkMessageBox(&m_Socket, &newMsg);

            if (newMsg.type != AnswerForAsk)
                handleMsg(newMsg);

            else
            {
                std::cerr << "Mamy odpowiedz od ip:" << newMsg.text << "|\n";
                int j = 0;
                for (const auto &dev : m_devicesList)
                {

                    if (strcmp(dev.first.first.c_str(), newMsg.text) == 0)
                    {
                        if (dev.first.second == newMsg.sender_port)
                        {
                            accept_list[j] = true;
                            i--;
                            break;
                        }
                    }

                    else
                        j++;
                }
                newMsg.type = EmptyMessage;
            }
        }

        m_inCritical = true;
        m_curTimestamp = 0;
        m_GuiPtr -> setInCritical(true);
        std::cerr << "Jestesmy w sekcji krytycznej!\n";
    });
    thread.detach();
}

void Controller::leaveCriticalSection()
{
    m_inCritical = false;
    m_curTimestamp = 0;
    std::cerr << "Opuszczamy sekcje krytyczna!\n";
    m_GuiPtr -> setInCritical(false);
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

        while (m_LoopOn)
        {
            if (!m_IsReady)
                continue;

            if (m_curTimestamp == 0)
            {
            checkMessageBox(&m_Socket, &msg);

            if (msg.type != EmptyMessage)
                handleMsg(msg);
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
        break;

    case ConnectionAccepted:
        break;

    case NewDevice:
        std::cerr << "nowe urzadzenie! Addr:" << msg.text << ":" << msg.timestamp << std::endl;
        m_devicesList.insert(std::pair<addr, unsigned>(addr(msg.text, static_cast<unsigned>(msg.timestamp)), 0));
    break;

    case Disconnect:
        std::cerr << "Usunieto urzadzenie! Addr:" << msg.text << ":" << msg.sender_port << std::endl;
        m_devicesList.erase(addr(msg.text, msg.sender_port));
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
        if (!m_inCritical && m_curTimestamp == 0)
        {
            Message tmpMsg;
            tmpMsg.type = AnswerForAsk;
            std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, tmpMsg.text);
            std::cerr << "Moje ip:" << msg.text << std::endl;
            sendMessage(&m_Socket, &tmpMsg, m_port, msg.text, &msg.sender_port);
        }

        else
        {
            std::thread tmpThread([&]()
            {
               while (m_curTimestamp != 0) {}
               std::cerr << "Krok 1 - curTimestamp == 0\n";
               while (m_inCritical)
               {
                    std::cerr << m_inCritical << "|";
               }
               std::cerr << "Krok 2 - m_inCritical == false\n";

               Message tmpMsg;
               tmpMsg.type = AnswerForAsk;
               std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, tmpMsg.text);
               sendMessage(&m_Socket, &tmpMsg, m_port, msg.text, &msg.sender_port);
            });

            tmpThread.detach();
        }
    }

    break;

    case AnswerForAsk:
        break;

    case LeaveCritical:
        break;
    }

    msg.type = EmptyMessage;
}

void Controller::handleConnectionRequest(std::string ip, unsigned port)
{
    Message msg;
    msg.type = NewDevice;
    std::cerr << "rozmiar listy urzadzen:" << m_devicesList.size() << std::endl;
    std::cerr << "ip nowego urzadzenia:" << ip << ":" << port << std::endl;
    for (const auto &dev : m_devicesList)
    {
        msg.timestamp = dev.first.second;
        std::copy(dev.first.first.c_str(), dev.first.first.c_str() + MSG_MAX_LEN, msg.text);
        sendMessage(&m_Socket, &msg, m_port, ip.c_str(), &port);
    }

    msg.timestamp = port;
    std::copy(ip.c_str(), ip.c_str() + MSG_MAX_LEN, msg.text);
    for (const auto &dev : m_devicesList)
    {
        if (dev.first.first != m_HostIp && dev.first.second != m_port)
            sendMessage(&m_Socket, &msg, m_port, dev.first.first.c_str(), &dev.first.second);
    }
    m_devicesList.insert(std::pair<addr, unsigned>(addr(ip, port), 0));
    msg.type = ConnectionAccepted;
    sendMessage(&m_Socket, &msg, m_port, ip.c_str(), &port);
    std::cerr << "Nowe urządzenie\n";
}
