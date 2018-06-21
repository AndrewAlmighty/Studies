#include "berkeleymanager.h"

BerkeleyManager::BerkeleyManager()
{
    m_network = std::unique_ptr<NetworkManager>(new NetworkManager(m_device));
    m_clock = std::unique_ptr<Clock>(new Clock());
}

bool BerkeleyManager::RunAsServer()
{
    return true;
}

bool BerkeleyManager::RunAsClient(std::string ip)
{
    return true;
}

void BerkeleyManager::Stop()
{
    m_device.setID(-1);
}

void BerkeleyManager::setTime(std::string time)
{

}

int BerkeleyManager::getID() const
{
    return m_device.getID();
}

std::string BerkeleyManager::getMode() const
{
    return m_device.getMode();
}

std::string BerkeleyManager::getIP() const
{
    return m_device.getIP();
}

std::string BerkeleyManager::getMAC() const
{
    return m_device.getMAC();
}

std::string BerkeleyManager::getTime() const
{
    return std::string("asdf");
}
