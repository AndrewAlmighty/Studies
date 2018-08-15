#include "berkeleymanager.h"
#include "guimanager.h"

#include <thread>
#include <unistd.h>

BerkeleyManager::BerkeleyManager()
{
    m_network = std::unique_ptr<NetworkManager>(new NetworkManager());
    m_clock = std::unique_ptr<Clock>(new Clock());
}

bool BerkeleyManager::prepareToRunAsServer(int port)
{
    if(m_network -> createServer(&port))
        return true;

    else return false;
}

bool BerkeleyManager::prepareToRunAsClient(std::string ip, int port)
{
    if(m_network -> connectTo(ip, port) == true)
        return true;

    else return false;
}

void BerkeleyManager::detectServers()
{
    //To implement
}

void BerkeleyManager::start()
{
    m_clock -> setSystemTime();

    if(m_network -> getDevice().getModeStr().compare("server") == 0)
        runAsServer();

    else
        runAsClient();
}

bool BerkeleyManager::stop()
{
    if(m_network -> shutdownConnection() == false)
        return false;

    m_network -> resetIDCounter();
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
        m_network -> handleConnectionRefuseMessage();
        msg -> type = EmptyMessage;
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

        updateGui("Working");
        updateDevicesList();
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
        {
            updateGui("Connected");

            while(GuiManager::GetInstance().running() == true)
            {
                m_network -> checkMailBox(&msg);
                handleMessage(&msg);
            }
        }

        else
            breakAll();
   });

    threadObj.detach();
}

void BerkeleyManager::updateGui(std::string status)
{
    GuiManager::GetInstance().setID(getID());
    GuiManager::GetInstance().setMode(QString(getMode().c_str()));

    if(!status.empty())
        GuiManager::GetInstance().setStatus(QString::fromStdString(status));
}

void BerkeleyManager::updateDevicesList()
{

}

void BerkeleyManager::breakAll()
{
    //Something went wrong. Stop everything. Restart
    fprintf(stderr, "Critical error. Stopping everything!!\n");
    GuiManager::GetInstance().finishJob();
    GuiManager::GetInstance().loadSetupWindow();
}

bool BerkeleyManager::makingConnection(struct Message *msg)
{
    updateGui("Connecting ...");
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
