#include "networkmanager.h"
#include "networkmethods.h"

NetworkManager::NetworkManager(Device &dev)
{
    dev.setMac(readMac());
    dev.setIp(readIP());
    runUDPServer();
}

std::string NetworkManager::readMac()
{
    return std::string("78:32:1b:03:3a:71");
}

std::string NetworkManager::readIP()
{
    return std::string("192.168.1.106");
}
