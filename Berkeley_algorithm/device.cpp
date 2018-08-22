#include "device.h"

Device::Device()
{
    resetDevice();
}

Device::Device(const Device &dev)
{
    m_ip = dev.getIP();
    m_mac = dev.getMAC();
    m_ifc = dev.getInterface();
    m_ID = dev.getID();
    m_mode = dev.getMode();
    m_ready = dev.isReady();
}

bool Device::operator==(const Device &dev)
{
    if(dev.getID() == this -> m_ID)
        return true;

    else return false;
}

void Device::setIPandIfc(std::string s)
{
    std::string tmp;
    for(unsigned i = 0; i < s.length(); i++)
    {
        if(s[i] != '_')
            tmp += s[i];

        else
        {
            m_ip = tmp;
            tmp.clear();
            continue;
        }
    }

    m_ifc = tmp;
}

void Device::setIP(std::string ip)
{
    if(m_ip == ip)
        return;

    m_ip = ip;
}

void Device::setMac(std::string mac)
{
    if(m_mac == mac)
        return;

    m_mac = mac;
}

void Device::setID(int id)
{
    if(m_ID == id)
        return;

    m_ID = id;
}

void Device::setMode(Mode m)
{
    if(m_mode == m)
        return;

    m_mode = m;
}

void Device::setModeFromString(std::string mode)
{
    if(mode == "server")
        m_mode = Server;

    else if(mode == "client")
        m_mode = Client;

    else
        m_mode = NotSpecified;
}

void Device::resetDevice()
{
    m_ip = "Not Specified";
    m_mac = "Not Specified";
    m_ifc = "Not Spiecified";
    m_ID = -1;
    m_mode = NotSpecified;
    m_ready = false;
}

void Device::setReady(bool isReady)
{
    if(m_ready != isReady)
        m_ready = isReady;
}

int Device::getID() const
{
    return m_ID;
}

std::string Device::getIP() const
{
    return m_ip;
}

std::string Device::getMAC() const
{
    return m_mac;
}

std::string Device::getModeStr() const
{
    if(m_mode == Server)
        return std::string("server");

    else if(m_mode == Client)
        return std::string("client");

    else
        return std::string("Not Specified");
}

Device::Mode Device::getMode() const
{
    return m_mode;
}

std::string Device::getInterface() const
{
    return m_ifc;
}

bool Device::isReady() const
{
    return m_ready;
}
