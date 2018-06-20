#include "guimanager.h"
#include <QDebug>

GuiManager & GuiManager::GetInstance()
{
    static GuiManager instance;
    return instance;
}

GuiManager::GuiManager(QObject *parent) : QObject(parent)
{
    m_mode = "Server";
    m_time = "00:00:00";
    m_ip = "127.0.0.1";
    m_mac = "jakis mac";
}

void GuiManager::setTime(QString time)
{
    if(m_time == time)
        return;

    m_time = time;
    emit timeChanged();
    qDebug() << "Time: " << m_time;
}

QString GuiManager::time() const
{
    return m_time;
}

void GuiManager::setIp(QString ip)
{
    if(m_ip == ip)
        return;

    m_ip = ip;
    emit ipChanged();
    qDebug() << "IP: " << m_ip;
}

QString GuiManager::ip() const
{
    return m_ip;
}

void GuiManager::setMode(QString mode)
{
    if(m_mode == mode)
        return;

    m_mode = mode;
    emit modeChanged();
    qDebug() << "Mode: " << m_mode;
}

QString GuiManager::mode() const
{
    return m_mode;
}

void GuiManager::setServerIP(QString ip)
{
    if(m_serverIP == ip)
        return;

    m_serverIP = ip;
    emit serverIPChanged();
    qDebug() << "Server IP: " << m_serverIP;
}

QString GuiManager::serverIP() const
{
    return m_serverIP;
}

void GuiManager::setMAC(QString mac)
{
    if(m_mac == mac)
        return;

    m_mac = mac;
    emit MACChanged();
    qDebug() << "MAC:" << m_mac;
}

QString GuiManager::MAC() const
{
    return m_mac;
}
