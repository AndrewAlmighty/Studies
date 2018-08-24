#include "berkeleymanager.h"
#include "guimanager.h"

#include <thread>
#include <unistd.h>
#include <algorithm>

BerkeleyManager::BerkeleyManager()
{
    m_network = std::unique_ptr<NetworkManager>(new NetworkManager());
    m_clock = std::unique_ptr<Clock>(new Clock());
    m_clientsCount = 0;
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
            updateDevicesList(addDevice, newDevice);

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
        fprintf(stderr, "ADJUST TIME REQUEST: TIME:%s\n", msg -> message);
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
        updateDevicesList(BerkeleyManager::addDevice, m_network -> getDevice());
        struct Message msg;
        while(GuiManager::GetInstance().running() == true)
        {
            m_network -> checkMailBox(&msg);
            handleMessage(&msg);
            if(m_clock -> didCheckTimePassed() == true)
                RequestTimeFromClients();
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

void BerkeleyManager::updateDevicesList(BerkeleyManager::updateListAction action, const Device &dev)
{
    if(action == addDevice)
        GuiManager::GetInstance().addDevice(dev.getID(), dev.getIP().c_str(), dev.getMAC().c_str(), dev.getModeStr().c_str());

    else if(action == removeDevice)
        GuiManager::GetInstance().removeDevice(dev.getID());

    else
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
        m_clientsCount++;
        if(m_clientsCount == m_clientsID.size())
            return true;
    }

    if(m_clock -> isItTooLateForCheck() == true)
    {
        //disconect late devices
        return true;
    }

    return false;
}

void BerkeleyManager::sendAdjustTimeRequest()
{
    int n = 0, s_sum = 0, m_sum = 0, h_sum = 0, s, m, h;

    for(auto it = m_times.begin(); it != m_times.end(); ++it)
    {
        m_clock -> getSecMinHour(s, m, h, *it);
        n++;
        s_sum += s;
        m_sum += m;
        h_sum += h;
        fprintf(stderr, "GODZINA:%d:%d:%d\n",h,m,s);
    }

    s_sum = s_sum / n;
    m_sum = m_sum / n;
    h_sum = h_sum / n;
    fprintf(stderr, "Srednia godzina:%d:%d:%d\n",h_sum,m_sum,s_sum);
    std::string newTime;
    newTime = std::to_string(h_sum);
    newTime += ':';
    newTime += std::to_string(m_sum);
    newTime += ':';
    newTime += std::to_string(s_sum);
    m_network -> sendAdjustTimeRequest(newTime);
}

void BerkeleyManager::RequestTimeFromClients()
{
    m_clientsCount = 0;
    m_clientsID.clear();
    m_times.clear();

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
