#include "networkmanager.h"
#include "networkmethods.h"

NetworkManager::NetworkManager(Device &dev)
{
    dev.setMac(readMac());
    dev.setIp(readIP());
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

bool NetworkManager::connectToServer(std::string ip)
{

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

std::string NetworkManager::readMac()
{
    return std::string("78:32:1b:03:3a:71");
}

std::string NetworkManager::readIP()
{
    return std::string("192.168.1.106");
}
