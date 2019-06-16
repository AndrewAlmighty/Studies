#include "devicemodel.hpp"

DeviceModel::DeviceModel(QString ip, unsigned port, QString timestamp, QObject *parent) : QObject(parent)
{
    m_ip = ip;
    m_port = port;
    m_timestamp = timestamp;
}

QString DeviceModel::IP() const
{
    return m_ip;
}

unsigned DeviceModel::port() const
{
    return m_port;
}

QString DeviceModel::timestamp() const
{
    return m_timestamp;
}

void DeviceModel::setIP(QString ip)
{
    if (m_ip == ip)
        return;

    m_ip = ip;
}

void DeviceModel::setPort(unsigned port)
{
    if (m_port == port)
        return;

    m_port = port;
}

void DeviceModel::setTimestamp(QString timestamp)
{
    if (m_timestamp == timestamp)
        return;

    m_timestamp = timestamp;
}
