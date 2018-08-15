#include "berkeleymanager.h"
#include "guimanager.h"

#include <thread>

BerkeleyManager::BerkeleyManager()
{
    m_network = std::unique_ptr<NetworkManager>(new NetworkManager());
    m_clock = std::unique_ptr<Clock>(new Clock());
}

bool BerkeleyManager::PrepareToRunAsServer(int port)
{
    if(m_network -> createServer(&port))
        return true;

    else return false;
}

bool BerkeleyManager::PrepareToRunAsClient(std::string ip, int port)
{
    if(m_network -> connectTo(ip, port) == true)
        return true;

    else return false;
}

void BerkeleyManager::DetectServers()
{
/*
    std::thread threadObj([]{
        while(GuiManager::GetInstance().detectServers() == true)
        {

        }
   });

   threadObj.detach();
*/
}

void BerkeleyManager::start()
{
    m_clock -> setSystemTime();

    if(m_network -> getDevice().getModeStr() == "server")
        runAsServer();

    else
        runAsClient();
}

bool BerkeleyManager::Stop()
{
    if(m_network ->shutdownConnection() == false)
        return false;

    return true;
}

void BerkeleyManager::setTime(std::string time)
{
    m_clock -> setTime(time);
}

int BerkeleyManager::getID() const
{
    return m_network -> getDevice().getID();
}

std::string BerkeleyManager::getMode() const
{
    return m_network -> getDevice().getModeStr();
}

std::string BerkeleyManager::getIP() const
{
    return m_network -> getDevice().getIP();
}

std::string BerkeleyManager::getMAC() const
{
    return m_network -> getDevice().getMAC();
}

std::string BerkeleyManager::getTime() const
{
    return m_clock -> getTime();
}

void BerkeleyManager::respondForMessage(const Message *msg)
{
    //Here we check what kind of message came and then we react for it.
    switch(msg -> type)
    {
    case EmptyMessage:
        break;

    case ConnectionRequest:
        m_network -> handleConnectionRequest(msg);
        break;

    case ConnectionAccepted:
        return;

    case ConnectionRefused:
        return;

    case Disconnect:
        return;

    case ClientsCheck:
        return;

    case ClientConfirm:
        return;

    case ClientTime:
        return;

    case TimeRequest:
        return;

    case TimeAdjustRequest:
        return;
    }
}

void BerkeleyManager::runAsServer()
{
    std::thread threadObj([&]{

        struct Message msg;
        while(GuiManager::GetInstance().running() == true)
        {
            m_network -> checkMailBox(&msg);
            respondForMessage(&msg);
        }
   });

   threadObj.detach();
}

void BerkeleyManager::runAsClient()
{
    std::thread threadObj([&]{

        struct Message msg;
        while(GuiManager::GetInstance().running() == true)
        {
            m_network -> checkMailBox(&msg);
            respondForMessage(&msg);
        }
   });

   threadObj.detach();
}
