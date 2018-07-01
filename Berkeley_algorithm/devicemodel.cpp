#include "devicemodel.h"

DeviceModel::DeviceModel(int id, QString ip, QString mac, QString mode, QObject *parent) : QObject(parent)
{
    m_id = id;
    m_ip = ip;
    m_mac = mac;
    m_mode = mode;
}

int DeviceModel::ID() const
{
    return m_id;
}

QString DeviceModel::IP() const
{
    return m_ip;
}

QString DeviceModel::MAC() const
{
    return m_mac;
}

QString DeviceModel::Mode() const
{
    return m_mode;
}

void DeviceModel::setID(int id)
{
    if(m_id == id)
        return;

    m_id = id;
}

void DeviceModel::setIP(QString ip)
{
    if(m_ip == ip)
        return;

    m_ip = ip;
}

void DeviceModel::setMAC(QString mac)
{
    if(m_mac == mac)
        return;

    m_mac = mac;
}

void DeviceModel::setMode(QString mode)
{
    if(m_mode == mode)
        return;

    m_mode = mode;
}
