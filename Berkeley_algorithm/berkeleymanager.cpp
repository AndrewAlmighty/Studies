#include "berkeleymanager.h"
#include "guimanager.h"

#include <thread>
#include <QDebug>

BerkeleyManager::BerkeleyManager()
{
    m_network = std::unique_ptr<NetworkManager>(new NetworkManager(m_device));
    m_clock = std::unique_ptr<Clock>(new Clock());
}

bool BerkeleyManager::PrepareToRunAsServer()
{
    m_device.setMode(Device::Server);
    return true;
}

bool BerkeleyManager::PrepareToRunAsClient(std::string ip)
{
    m_device.setMode(Device::Client);
    return true;
}

void BerkeleyManager::start()
{
    m_clock -> setSystemTime();
/*
    std::thread threadObj([]{
        while(GuiManager::GetInstance().running() == true)
        {

        }
   });

    threadObj.detach();
    */
}

void BerkeleyManager::Stop()
{
    m_device.setID(-1);
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
