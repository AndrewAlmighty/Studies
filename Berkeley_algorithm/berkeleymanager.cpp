#include "berkeleymanager.h"
#include "guimanager.h"

#include <thread>

BerkeleyManager::BerkeleyManager()
{
    m_network = std::unique_ptr<NetworkManager>(new NetworkManager(m_device));
    m_clock = std::unique_ptr<Clock>(new Clock());
}

bool BerkeleyManager::PrepareToRunAsServer(int port)
{
    m_device.setMode(Device::Server);

    if(m_network -> createServer(port))
        return true;

    else return false;
}

bool BerkeleyManager::PrepareToRunAsClient(std::string ip)
{
    m_device.setMode(Device::Client);
    m_network -> connectTo(ip, 9000);
    return true;
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

    std::thread threadObj([&]{
        while(GuiManager::GetInstance().running() == true)
        {
            m_network -> checkMailBox();
        }
   });

   threadObj.detach();
}

bool BerkeleyManager::Stop()
{
    m_device.setID(-1);
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
    return m_clock -> getTime();
}
