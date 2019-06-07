#include "controller.hpp"
#include "guicontroller.hpp"

#include <chrono>
#include <thread>

Controller::Controller(GuiController *ptr)
{
    m_GuiPtr = ptr;
    loop();
}

Controller::~Controller()
{
    m_LoopOn = false;
    shutdownSocket(&m_Socket);
}

bool Controller::connectTo(const std::string &ip, const unsigned &port)
{
    Message msg;
    msg.type = EmptyMessage;
    msg.sender_port = m_port;
    std::copy(m_HostIp.c_str(), m_HostIp.c_str() + MSG_MAX_LEN, msg.text);
    sendConnectionRequest(&m_Socket, ip.c_str(), &port, &msg);
    msg.type = EmptyMessage;

    auto start_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time);

    while (duration.count() < 3)
    {
        duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time);
        checkMessageBox(&m_Socket, &msg);

        if (msg.type == ConnectionAccepted)
            return true;
    }

    return false;
}

bool Controller::setNetworking(const std::string &ip, const unsigned &port)
{
    if (createAndBindSocket(&m_Socket, &port))
    {
        m_IsReady = true;
        m_HostIp = ip;
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

}

void Controller::leaveCriticalSection()
{

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

            checkMessageBox(&m_Socket, &msg);

            if (msg.type != EmptyMessage)
                handleMsg(msg);
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
    }

    msg.type = EmptyMessage;
}
