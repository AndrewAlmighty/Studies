#include "berkeleymanager.h"
#include "guimanager.h"

#include <thread>
#include <unistd.h>

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

bool BerkeleyManager::handleMessage(struct Message *msg)
{
    //Here we check what kind of message came and then we react for it.
    switch(msg -> type)
    {
    case EmptyMessage:
        return false;

    case ConnectionRequest:
        m_network -> handleConnectionRequest(msg);
        msg -> type = EmptyMessage;
        return true;

    case ConnectionAccepted:
        m_network -> handleConnectionAcceptedMessage(msg);
        msg -> type = EmptyMessage;
        return true;

    case ConnectionRefused:
        return false;

    case Disconnect:
        return false;

    case ClientsCheck:
        return false;

    case ClientConfirm:
        return false;

    case ClientTime:
        return false;

    case TimeRequest:
        return false;

    case TimeAdjustRequest:
        return false;
    }

    return false;
}

void BerkeleyManager::runAsServer()
{
    std::thread threadObj([&]{

        struct Message msg;
        while(GuiManager::GetInstance().running() == true)
        {
            m_network -> checkMailBox(&msg);
            handleMessage(&msg);
        }
   });

   threadObj.detach();
}

void BerkeleyManager::runAsClient()
{
    std::thread threadObj([&]{

        struct Message msg;
        msg.type = EmptyMessage;
        if(makingConnection(&msg) == true)
            updateGui();

        while(GuiManager::GetInstance().running() == true)
        {
            m_network -> checkMailBox(&msg);
            handleMessage(&msg);
        }

   });

    threadObj.detach();
}

void BerkeleyManager::updateGui()
{
    GuiManager::GetInstance().setID(getID());
    GuiManager::GetInstance().setMode(QString(getMode().c_str()));
}

bool BerkeleyManager::makingConnection(struct Message *msg)
{
    int maxTime = 10;   //we wait no more than 10 second to connect
    while(maxTime > 0)
    {
        m_network -> checkMailBox(msg);
        if(handleMessage(msg) == true)
            return true;

        sleep(1);
        maxTime--;
    }

    return false;
}
