#include "berkeleymanager.h"
#include "guimanager.h"

#include <thread>
#include <unistd.h>
#include <algorithm>

BerkeleyManager::BerkeleyManager()
{
    m_network = std::unique_ptr<NetworkManager>(new NetworkManager());
    m_clock = std::unique_ptr<Clock>(new Clock());
    m_clientsCheck = false;
    m_timeCheck = false;
}

bool BerkeleyManager::prepareToRunAsServer(int port)
{
    m_network -> prepare();

    if(m_network -> createServer(&port))
        return true;

    else return false;
}

bool BerkeleyManager::prepareToRunAsClient(std::string ip, int port)
{
    m_network -> prepare();

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

    m_network -> reset();
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
    {
        Device newDevice;
        if(m_network -> handleConnectionRequest(msg, newDevice) == true)
            addDeviceToGuiDevicesList(newDevice);

        msg -> type = EmptyMessage;
        return true;
    }

    case ConnectionAccepted:
        m_network -> handleConnectionAcceptedMessage(msg);
        msg -> type = NetworkSizeRequest;
        strcpy(msg -> message, "ID:");
        strcat(msg -> message, std::to_string(m_network -> getDevice().getID()).c_str());
        m_network -> sendMsg(msg);
        msg -> type = EmptyMessage;
        return true;

    case ConnectionRefused:
        m_network -> handleConnectionRefuseMessage();
        breakAll();
        msg -> type = EmptyMessage;
        return false;

    case Disconnect:
        m_network -> shutdownConnection();
        m_network -> reset();
        msg -> type = EmptyMessage;
        breakAll();
        return false;

    case NetworkSizeRequest:
        m_network -> handleNetworkSizeRequest(msg);
        msg -> type = EmptyMessage;
        return false;

    case NetworkSize:
    {
        int size = m_network -> handleNetworkSize(msg);
        updateGui("Connected ... getting devices list");
        m_network -> getDevices(msg, size);
        setGuiDevicesList();
        updateGui("Connected");
        msg -> type = ClientReady;
        m_network -> sendMsg(msg);
        msg -> type = EmptyMessage;
        return false;
    }

    case ClientReady:
    {
        m_network -> handleClientReadyMsg(msg);
        msg -> type = EmptyMessage;
        return false;
    }
    case DeviceInfoRequest:
        m_network -> handleDeviceInfoRequest(msg);
        msg -> type = EmptyMessage;
        return false;

    case DeviceInfo:
        return false;

    case ClientsCheck:
        return false;

    case ClientConfirm:
        return false;

    case ClientTime:
        m_times.push_back(std::string(msg -> message));
        if(checkIfAllClientsSendTime(msg -> sender_id) == true)
            sendAdjustTimeRequest();

        msg -> type = EmptyMessage;

        return false;

    case TimeRequest:
        m_network -> handleTimeRequest(msg, m_clock -> getTime());
        msg -> type = EmptyMessage;
        return false;

    case TimeAdjustRequest:
        m_clock -> setTime(msg -> message);
        msg -> type = EmptyMessage;
        return false;
    }

    return false;
}

void BerkeleyManager::runAsServer()
{
    std::thread threadObj([&]{

        updateGui("Working");
        addDeviceToGuiDevicesList(m_network -> getDevice());
        struct Message msg;
        while(GuiManager::GetInstance().running() == true)
        {
            m_network -> checkMailBox(&msg);
            handleMessage(&msg);

            if(m_timeCheck == false)
            {
                if(m_clock -> isItTimeToCheckTime() == true)
                    RequestTimeFromClients();
            }

            else if(m_timeCheck == true)
            {
                    if(isItTimeToAdjustTime() == true)
                        sendAdjustTimeRequest();
            }

            if(m_clientsCheck == false)
            {

            }
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

void BerkeleyManager::addDeviceToGuiDevicesList(const Device &dev)
{
    GuiManager::GetInstance().addDevice(dev.getID(), dev.getIP().c_str(), dev.getMAC().c_str(), dev.getModeStr().c_str());
}

void BerkeleyManager::removeDeviceFromGuiDevicesList(const int &id)
{
    GuiManager::GetInstance().removeDevice(id);
}

void BerkeleyManager::clearGuiDevicesList()
{
     GuiManager::GetInstance().removeAllDevices();
}

void BerkeleyManager::setGuiDevicesList()
{
    int i = 0;
    std::list<Device>::const_iterator it;
    while(m_network -> getDevicesList(it, i) == true)
    {
        GuiManager::GetInstance().addDevice(it -> getID(), it -> getIP().c_str(), it -> getMAC().c_str(), it -> getModeStr().c_str());
        i++;
    }
}

void BerkeleyManager::breakAll()
{
    //Something went wrong. Stop everything. Restart
    fprintf(stderr, "Critical error. Stopping everything!!\n");
    GuiManager::GetInstance().finishJob();
    GuiManager::GetInstance().loadSetupWindow();
}

bool BerkeleyManager::checkIfAllClientsSendTime(const int &id)
{
    std::list<int>::iterator it;
    if((it = std::find(m_clientsID.begin(), m_clientsID.end(), id)) != m_clientsID.end())
    {
        m_clientsID.remove(*it);
        if(m_clientsID.empty() == true)
            return true;
    }

    if(isItTimeToAdjustTime() == true)
        return true;

    return false;
}

void BerkeleyManager::sendAdjustTimeRequest()
{
    int n = 0, s_sum = 0, s, m, h;

    for(auto it = m_times.begin(); it != m_times.end(); ++it)
    {
        m_clock -> getSecMinHour(s, m, h, *it);
        n++;
        s_sum += s;
        s_sum += (m * 60);
        s_sum += (h * 60 * 60);
    }

    s_sum = s_sum / n;
    h = s_sum / (60 * 60);
    s_sum -= (h * 60 * 60);
    m = s_sum / 60;
    s = s_sum % 60;

    std::string newTime;
    newTime = std::to_string(h);
    newTime += ':';
    newTime += std::to_string(m);
    newTime += ':';
    newTime += std::to_string(s);
    m_clock -> setTime(newTime);
    m_network -> sendAdjustTimeRequest(newTime);
    m_timeCheck = false;
}

void BerkeleyManager::RequestTimeFromClients()
{
    m_clientsID.clear();
    m_times.clear();
    m_timeCheck = true;
    m_clientsCheck = false;

    int i = 0;
    std::list<Device>::const_iterator it;
    while(m_network -> getDevicesList(it, i) == true)
    {
        if(it -> isReady() == true && it -> getModeStr() == "client")
            m_clientsID.push_back(it -> getID());

        i++;
    }

    m_times.push_back(m_clock -> getTime());
    m_network -> sendRequestTime();
}

bool BerkeleyManager::isItTimeToAdjustTime()
{
    if(m_clock -> isItTooLateForCheck() == true)
    {
        for(auto it = m_clientsID.begin(); it != m_clientsID.end(); ++it)
        {
            m_network -> disconnectDevice(*it);
            removeDeviceFromGuiDevicesList(*it);

        }
        return true;
    }

    return false;
}

bool BerkeleyManager::makingConnection(struct Message *msg)
{
    updateGui("Connecting ...");
    int maxTime = 10;   //we wait no more than 10 second to connect
    while(maxTime > 0)
    {
        m_network -> checkMailBox(msg);
        if(handleMessage(msg) == true && GuiManager::GetInstance().running() == true)
            return true;

        sleep(1);
        maxTime--;
    }

    m_network -> reset();
    return false;
}
