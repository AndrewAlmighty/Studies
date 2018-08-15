#include "networkmanager.h"
#include "networkmethods.h"

#include <ctype.h>
#include <cstring>

NetworkManager::NetworkManager()
{
    m_device.setIPandIfc(readIPandIfc());
    m_device.setMac(readMac(m_device.getInterface()));
    m_socket = -1;
    m_IDcounter = 0;
    m_port = -1;
}

bool NetworkManager::createServer(int *port)
{
    //check if we have already opened a socket.
    if(m_socket >= 0)
        return false;

    //Create a socket. Provide a ptr to socket and port.
    if(createAndBindSocket(&m_socket, port) == serverStatus::Running)
    {
        m_port = *port;
        m_device.setMode(Device::Server);
        m_device.setID(m_IDcounter);
        addDeviceToNetworkList(m_device.getIP(), m_device.getMAC(), m_device.getID(), m_device.getMode());
        return true;
    }

    else return false;
}

bool NetworkManager::connectTo(std::string ip, const int &port)
{
    int id = 0;
    if(connectToServer(&m_socket, ip.c_str(), &port, m_device.getIP().c_str(), m_device.getMAC().c_str(), &id) == true)
    {
        m_port = port;
        m_device.setMode(Device::Client);
        m_device.setID(id);
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

void NetworkManager::sendMsg(const Message *msg, const std::string &ip)
{
    sendMessage(&m_socket, msg, ip.c_str(), &m_port);
}

void NetworkManager::resetIDCounter()
{
    m_IDcounter = 0;
}

bool NetworkManager::handleConnectionRequest(const struct Message *msg)
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
        acceptClient(ip, mac);
        return true;
    }

    return false;
}

Device NetworkManager::getDevice() const
{
    return m_device;
}

void NetworkManager::acceptClient(const std::string &ip, const std::string &mac)
{
    //Adding new device to list of devices in network.
    addDeviceToNetworkList(ip, mac, m_IDcounter, Device::Client);

    //Send confirmation of connection request.
    struct Message msg;
    msg.type = ConnectionAccepted;
    msg.device_id = 1;
    strcpy(msg.message, "ID:2");
    sendMsg(&msg, ip);
}

void NetworkManager::addDeviceToNetworkList(const std::string &ip, const std::string &mac, const int id, Device::Mode mode)
{
    Device newDevice;
    newDevice.setIP(ip);
    newDevice.setMac(mac);
    newDevice.setMode(mode);
    newDevice.setID(id);
    m_deviceList.push_back(Device());
    m_IDcounter++;
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
