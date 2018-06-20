#include "berkeleymanager.h"

BerkeleyManager::BerkeleyManager()
{
    m_id = -1;  //when -1 for not specified
    m_mode = NotSpecified;
}

bool BerkeleyManager::RunAsServer()
{
    m_mode = Server;
    return true;
}

bool BerkeleyManager::RunAsClient(std::string ip)
{
    m_mode = Client;
    return true;
}

void BerkeleyManager::Stop()
{
    m_mode = NotSpecified;
    m_id = -1;
}

void BerkeleyManager::setMode(Mode m)
{
    if(m_mode == m)
        return;

    m_mode = m;
}
void BerkeleyManager::setID(int id)
{
    if(m_id == id)
        return;

    m_id = id;
}

void BerkeleyManager::setTime(std::string time)
{

}

int BerkeleyManager::getID() const
{
    return m_id;
}

std::string BerkeleyManager::getMode() const
{
    if(m_mode == Server)
        return std::string("server");

    else if(m_mode == Client)
        return std::string("client");

    else
        return std::string("NotSpecified");
}

std::string BerkeleyManager::getIP() const
{
    return std::string("cos1");
}

std::string BerkeleyManager::getMAC() const
{
    return std::string("cos2");
}

std::string BerkeleyManager::getTime() const
{
    return std::string("asdf");
}
