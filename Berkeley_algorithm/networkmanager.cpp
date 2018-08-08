#include "networkmanager.h"
#include "networkmethods.h"

NetworkManager::NetworkManager(Device &dev)
{
    dev.setIPandIfc(readIPandIfc());
    dev.setMac(readMac(dev.getInterface()));
    m_socket = -1;
}

bool NetworkManager::createServer(int port)
{
    //check if we have already opened a socket.
    if(m_socket >= 0)
        return false;

    //Create a socket. Provide a ptr to socket and port.
    if(createUDPServer(&m_socket, port) == serverStatus::Running)
        return true;

    else return false;
}

bool NetworkManager::connectTo(std::string ip, int port)
{
    connectToServer(&m_socket, ip.c_str(), port);
}

bool NetworkManager::shutdownConnection()
{
    //Shutdown a socket. Doesn't mean if it's working as server or client. After shutdown, delete the pointer to avoid leak.
    if(shutdownSocket(&m_socket) == true)
    {
        m_socket = -1;
        return true;
    }

    else return false;
}

bool NetworkManager::checkMailBox()
{
    checkMessageBox(&m_socket, 123);
    return false;
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
    char ip_addr[15], iface[5];
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
