#include "networkmanager.h"
#include "networkmethods.h"

#include <ctype.h>
#include <cstring>
#include <thread>
#include <unistd.h>

NetworkManager::NetworkManager()
{
    reset();
}

bool NetworkManager::createServer(int *port)
{
    //check if we have already opened a socket.
    if(m_socket >= 0)
        return false;

    //Create a socket. Provide a ptr to socket and port.
    if(createAndBindSocket(&m_socket, port) == Working)
    {
        m_port = *port;
        fprintf(stderr,"Stworzono socket!\n");
        actionOnNetworkDevicesList(addDeviceToList, m_IDcounter, &m_device, m_device.getIP(), m_device.getMAC(), Device::Server);
        return true;
    }

    else return false;
}

bool NetworkManager::connectTo(std::string ip, const int &port)
{
    if(sendConnectionRequest(&m_socket, ip.c_str(), &port, m_device.getIP().c_str(), m_device.getMAC().c_str()) == Working)
    {
        m_port = port;
        m_device.setMode(Device::Client);
        m_server_ip = ip;
        return true;
    }

    else
        return false;
}

bool NetworkManager::shutdownConnection()
{
    //Shutdown a socket. Doesn't mean if it's working as server or client. After shutdown, delete the pointer to avoid leak.
    if(shutdownSocket(&m_socket) == true)
    {
        m_port = -1;
        m_socket = -1;
        return true;
    }

    else return false;
}

void NetworkManager::checkMailBox(struct Message *msg)
{
    checkMessageBox(&m_socket, msg);
}

void NetworkManager::sendMsg(struct Message *msg, const std::string &ip)
{
    if(ip.empty() == false)
        sendMessage(&m_socket, msg, m_device.getID(), ip.c_str(), &m_port);

    else
        sendMessage(&m_socket, msg, m_device.getID(), m_server_ip.c_str(), &m_port);
}

void NetworkManager::sendRequestTime()
{
    struct Message msg;
    msg.type = TimeRequest;
    for(auto it = m_deviceList.begin(); it != m_deviceList.end(); ++it)
        if(it -> isReady() == true && it -> getMode() == Device::Client)
            sendMsg(&msg, it -> getIP());
}

void NetworkManager::sendAdjustTimeRequest(const std::string &time)
{
    fprintf(stderr, "Wysylamy godzine :%s\n", time.c_str());
    struct Message msg;
    msg.type = TimeAdjustRequest;
    msg.sender_id = m_device.getID();
    strcpy(msg.message, time.c_str());

    for(std::list<Device>::const_iterator it = m_deviceList.begin(); it != m_deviceList.end(); ++it)
    {
        if(it -> getMode() == Device::Server || it -> isReady() == false)
            continue;

        sendMsg(&msg, it -> getIP());
    }
}

void NetworkManager::reset()
{
    m_device.resetDevice();
    m_socket = -1;
    m_IDcounter = 0;
    m_port = -1;
    m_server_ip.clear();
    m_deviceList.clear();
}

void NetworkManager::resetServerIP(const std::string ip)
{
    if(ip != m_server_ip)
        m_server_ip = ip;
}

bool NetworkManager::handleConnectionRequest(const struct Message *msg, Device &dev)
{
    /* Here we handle client's request for connection.
     * First, we check if in message there is IP and MAC of client. All this magic is in the for loop which is below.
     * Format of message here should be: IP:xxx.xxx.xxx.xxx_MAC:YY:YY:YY:YY:YY:YY
     */

    //first get size of message
    unsigned msgSize = strlen(msg -> message);

    std::string ip, mac;
    bool foundIp = false, foundMac = false, isIp = false, isMAC = false;

    for(unsigned i = 0; i < msgSize; i++)
    {
        //First, check if there is IP
        if(isIp == false)
        {
            ip += msg -> message[i];

            if(ip == "IP:")
            {
                isIp = true;
                ip.clear();
                continue;
            }
        }

        //We know now that there is IP in message, now copy whole string up to '_'
        if(isIp == true && msg -> message[i] != '_' && foundIp == false)
        {
            ip += msg -> message[i];
            continue;
        }

        //When we found '_' we check if IP is good, we check if there are no letters etc.
        else if(isIp == true && msg -> message[i] == '_' && foundIp == false)
        {
            //check if ip is in format xxx.xxx.xxx.xxx
            int tmp1 = 0, tmp2 = 0;
            bool isOK = true;
            for(unsigned j = 0; j < ip.size(); j++)
            {
                if(isdigit(ip[j]) == true)
                    tmp1++;

                else if(ip[j] == '.')
                {
                    tmp1 = 0;
                    tmp2++;
                }

                else if(isalpha(ip[j]))
                {
                    isOK = false;
                    break;
                }

                if(tmp1 > 3 || tmp2 > 3)
                {
                    isOK = false;
                    break;
                }
            }

            if(isOK == true)
            {
                foundIp = true;
                continue;
            }

            else
                break;
        }

        //when we have IP, now we get MAC address from message.
        if(foundIp == true && isMAC == false)
        {
            mac += msg -> message[i];
            if(mac == "MAC:")
            {
                isMAC = true;
                mac.clear();
                continue;
            }
        }

        //There is MAC address in message, now let's get it
        else if(foundIp == true && isMAC == true)
        {
            mac += msg -> message[i];

            if(i != msgSize - 1)
                continue;

            else
            {
                //it's end of message, check if we have good MAC address.
                int tmp1 = 0, tmp2 = 0;
                bool isOK = true;
                for(unsigned j = 0; j < mac.size(); j++)
                {
                    if(mac[j] != ':')
                        tmp1++;
                    else if(mac[j] == ':' && tmp1 != 0)
                    {
                        tmp1 = 0;
                        tmp2++;
                    }

                    else if(mac[j] == ':' && tmp1 == 0)
                    {
                        isOK = false;
                        break;
                    }

                    if(tmp1 > 2 || tmp2 > 5)
                    {
                        isOK = false;
                        break;
                    }
                }

                if(isOK == true)
                    foundMac = true;
            }
        }
    }

    //if we have IP and MAC, accept request.
    if(foundIp == true && foundMac == true)
    {
        acceptClient(dev, ip, mac);
        return true;
    }

    return false;
}

bool NetworkManager::handleConnectionAcceptedMessage(const struct Message *msg)
{
    std::string tmp;
    bool IDbegin = false;
    for(unsigned i = 0; i < strlen(msg -> message); i++)
    {
        tmp += msg -> message[i];

        if(IDbegin == false && tmp == "ID:")
        {
            tmp.clear();
            IDbegin = true;
        }
    }

    m_device.setID(std::stoi(tmp));
    return true;
}

void NetworkManager::handleConnectionRefuseMessage()
{
    reset();
}

void NetworkManager::handleNetworkSizeRequest(struct Message *msg)
{
    strcpy(msg -> message, "NetworkSize:");
    strcat(msg -> message, std::to_string(m_deviceList.size()).c_str());
    msg -> type = NetworkSize;
    sendMsg(msg, getDeviceIp(msg -> sender_id));
}

void NetworkManager::handleClientReadyMsg(struct Message *msg)
{
    std::list<Device>::iterator it;
    getIterFromDevicesList(msg -> sender_id, it);
    it -> setReady(true);
}

bool NetworkManager::getDevices(struct Message *msg, const int &size)
{
    //we ask for info about all devices. If we don't have a message in short time we shutdown a connection
    std::string tmp;
    Device dev;
    int k = 0, deadline = 0;  //we count ';' with it.

    for(int i = 0; i < size; i++)
    {
        msg -> type = DeviceInfoRequest;
        strcpy(msg -> message, "ID:");
        strcat(msg -> message, std::to_string(i).c_str());
        sendMsg(msg);

        msg -> type = EmptyMessage;
        while(msg -> type != DeviceInfo)
        {
            checkMailBox(msg);

            sleep(1);

            if(deadline == 7)
                return false;

            deadline++;
        }

        deadline = 0;
        for(unsigned j = 0; j < strlen(msg -> message); j++)
        {
            if(msg -> message[j] != ';')
                tmp += msg -> message[j];

            if(tmp == "ID:" || tmp == "IP:" || tmp == "MAC:" || tmp == "MODE:")
            {
                k++;
                tmp.clear();
                continue;
            }

            if(k == 1 && msg -> message[j] == ';')
            {
                dev.setID(std::atoi(tmp.c_str()));
                tmp.clear();
                continue;
            }

            else if(k == 2 && msg -> message[j] == ';')
            {
                dev.setIP(tmp);
                tmp.clear();
                continue;
            }

            else if(k == 3 && msg -> message[j] == ';')
            {
                dev.setMac(tmp);
                tmp.clear();
                continue;
            }
        }
        //on the end set mode
        dev.setModeFromString(tmp);
        actionOnNetworkDevicesList(addDeviceToList, dev.getID(), &dev);
        dev.resetDevice();
        tmp.clear();
        k = 0;
    }

    return true;
}

int NetworkManager::handleNetworkSize(Message *msg)
{
    //first, get size of network from message.
    std::string tmp;
    bool gotSize = false;
    for(int i = 0; msg -> message[i] != '\0'; i++)
    {
        tmp += msg -> message[i];
        if(gotSize == false && tmp == "NetworkSize:")
        {
            tmp.clear();
            gotSize = true;
        }

        else if (gotSize == true && std::isdigit(msg -> message[i]) == false)
        {
            gotSize = false;
            break;
        }
    }

    int size = 0;
    if(gotSize == true)
        size = atoi(tmp.c_str());

    return size;
}

void NetworkManager::handleTimeRequest(struct Message *msg, const std::string time)
{
    msg -> type = ClientTime;
    strcpy(msg -> message, time.c_str());
    sendMsg(msg);
}

void NetworkManager::handleDeviceInfoRequest(struct Message *msg)
{
    std::string tmp;
    bool is_ok = true;
    for(unsigned i = 0; i < strlen(msg -> message); i++)
    {
        tmp += msg -> message[i];

        if(i < 3 && tmp == "ID:")
            tmp.clear();

        else if(i >= 3 && std::isdigit(msg -> message[i]) != true)
        {
            is_ok = false;
            break;
        }
    }

    if(is_ok == true)
    {
        msg -> type = DeviceInfo;
        strcpy(msg -> message, "ID:");
        Device dev;
        actionOnNetworkDevicesList(getDeviceFromList, std::atoi(tmp.c_str()), &dev);
        strcat(msg -> message, std::to_string(dev.getID()).c_str());
        strcat(msg -> message, ";IP:");
        strcat(msg -> message, dev.getIP().c_str());
        strcat(msg -> message, ";MAC:");
        strcat(msg -> message, dev.getMAC().c_str());
        strcat(msg -> message, ";MODE:");
        strcat(msg -> message, dev.getModeStr().c_str());
        sendMsg(msg, getIpFromList(msg ->sender_id));
        msg -> type = EmptyMessage;
    }

}

Device NetworkManager::getDevice() const
{
    return m_device;
}

bool NetworkManager::getDevicesList(std::list<Device>::const_iterator &it, const int &i) const
{
    int j = 0;
    for(it = m_deviceList.begin(); it != m_deviceList.end(); ++it)
    {
        if(j == i)
            return true;

        j++;
    }

    return false;
}

void NetworkManager::prepare()
{
    m_device.setIPandIfc(readIPandIfc());
    m_device.setMac(readMac(m_device.getInterface()));
    m_device.setReady(true);
}

void NetworkManager::acceptClient(Device &dev, const std::string &ip, const std::string &mac)
{
    //Send confirmation of connection request.
    struct Message msg;
    msg.type = ConnectionAccepted;
    msg.sender_id = m_IDcounter;
    strcpy(msg.message, "ID:");
    strcat(msg.message, std::to_string(m_IDcounter).c_str());
    sendMsg(&msg, ip);

    //Adding new device to list of devices in network.
    actionOnNetworkDevicesList(addDeviceToList, m_IDcounter, &dev, ip, mac, Device::Client);
}

void NetworkManager::actionOnNetworkDevicesList(NetworkManager::listAction action, const int &id, Device *dev, const std::string &ip, const std::string &mac, const Device::Mode mode)
{
    if(action == addDeviceToList && dev -> getMode() == Device::NotSpecified)
    {
    dev -> setIP(ip);
    dev -> setMac(mac);
    dev -> setMode(mode);
    dev -> setID(id);
    m_IDcounter++;

    if(mode == Device::Server)
        dev -> setReady(true);

    m_deviceList.push_back(*dev);
    }

    else if(action == addDeviceToList && dev -> getMode() != Device::NotSpecified)
    {
        if(dev -> getMode() == Device::Server)
            dev -> setReady(true);

        m_IDcounter++;
        m_deviceList.push_back(*dev);
    }

    else if(action == removeDeviceFromList)
    {
        for(auto it = m_deviceList.begin(); it != m_deviceList.end(); ++it)
        {
            if(id == it -> getID())
            {
                m_deviceList.remove(*it);
                return;
            }
        }
    }

    else if(action == getDeviceFromList)
    {
        for(auto it = m_deviceList.begin(); it != m_deviceList.end(); ++it)
        {
            if(id == it -> getID())
            {
                *dev = *it;
                return;
            }
        }
    }

    else if(action == clearList)
        m_deviceList.clear();
}

void NetworkManager::getIterFromDevicesList(const int &id, std::list<Device>::iterator &it)
{
    for(it = m_deviceList.begin(); it != m_deviceList.end(); ++it)
        if(id == it -> getID())
            return;
}


std::string NetworkManager::getIpFromList(const int &id)
{
    for(auto it = m_deviceList.begin(); it != m_deviceList.end(); ++it)
    {
        if(id == it -> getID())
            return it -> getIP();
    }

    return "There is no ID here!";
}

std::string NetworkManager::readMac(std::string ifc)
{
    char mac_addr[17];
    if(getMacAddr(mac_addr, ifc.c_str()) == CannotGetMacAddr)
        return std::string("MAC ADDR - ERROR");

    else return std::string(mac_addr);
}

std::string NetworkManager::readIPandIfc()
{
    //read IP and interface and return it. Need interface for MAC address.
    char ip_addr[15], iface[20];
    switch (getIPAndIFACE(ip_addr, iface))
    {
        case MoreThanOneIp:
        case CannotGetIP:
            return std::string("127.0.0.1");
        case IPFound:
        {
            std::string IpIface;
            IpIface += ip_addr;
            IpIface += "_";
            IpIface += iface;
            return IpIface;
        }
        default:
            return std::string("IP READ ERROR!");
    }
}

std::string NetworkManager::getDeviceIp(const int &id) const
{
    for(auto it = m_deviceList.begin(); it != m_deviceList.end(); ++it)
    {
        if(id == it -> getID())
        {
            return it -> getIP();
        }
    }

    return "Not found";
}
