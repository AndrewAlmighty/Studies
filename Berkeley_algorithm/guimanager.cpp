#include "guimanager.h"
#include <QDebug>

GuiManager & GuiManager::GetInstance()
{
    static GuiManager instance;
    return instance;
}

void GuiManager::setManager(BerkeleyManager &manager)
{
    m_berkeley = &manager;
}

GuiManager::GuiManager(QObject *parent) : QObject(parent)
{
    restartConfiguration();
    m_running = false;

    addDevice(1,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(2,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(3,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(4,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(5,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(6,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(7,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(8,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(9,"123:456:789:123", "78:32:1b:03:3a:71", "Server");
    addDevice(10,"123:456:789:123", "78:32:1b:03:3a:71", "Server");

    removeDevice(5);
}

void GuiManager::restartConfiguration()
{
    m_mode = "Server";
    m_time = "Not Specified";
    m_ip = "Not Specified";
    m_mac = "Not Specified";
    m_ID = -1;
}

void GuiManager::beginJob()
{
    if(m_mode == "Server")
    {
        qDebug() << "Prepare to run as Server!";
        if(m_berkeley -> PrepareToRunAsServer() == false)
            return;
    }

    else if(m_mode == "Client")
    {
        qDebug() << "Prepare to run as Client!";
        if(m_berkeley -> PrepareToRunAsClient(m_serverIP.toStdString()) == false)
                return;
    }

    else
        return;

    qDebug() << "Preparing done!";
    setID(m_berkeley -> getID());
    setMode(QString(m_berkeley -> getMode().c_str()));
    setMAC(QString(m_berkeley -> getMAC().c_str()));
    setIp(QString(m_berkeley -> getIP().c_str()));
    setRunning(true);
    m_berkeley -> start();
}

void GuiManager::finishJob()
{
    m_berkeley -> Stop();
    restartConfiguration();
    setRunning(false);
}

void GuiManager::changeClock(QString time)
{
    m_berkeley -> setTime(time.toStdString());
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
{qDebug() << "Device with ID disconnected!";
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

void GuiManager::setRunning(bool running)
{
    if(m_running == running)
        return;

    m_running = running;qDebug() << "Device with ID disconnected!";
    emit runningChanged();

    if(m_running == true)
    {
        qDebug() << "Running!";
        setTime(QString(m_berkeley -> getTime().c_str()));
    }

    else
        qDebug() << "Stopped!";
}

bool GuiManager::running() const
{
    return m_running;
}

void GuiManager::setID(int id)
{
    if(m_ID == id)
        return;

    m_ID = id;
    emit IDChanged();
    qDebug() << "ID: " << m_ID;
}

int GuiManager::ID() const
{
    return m_ID;
}

QList<QObject *> GuiManager::model() const
{
    return m_deviceModel;
}

void GuiManager::addDevice(int id, QString ip, QString mac, QString mode)
{
    m_deviceModel.append(new DeviceModel(id, ip, mac, mode));
    emit modelChanged();
    qDebug() << "New device connected! ID: " + QString::number(id) + " IP:" + ip + " MAC:" + mac + " Mode:" + mode;
}

bool GuiManager::removeDevice(int id)
{
    for(int i = 0; i < m_deviceModel.size(); i++)
    {
        if(m_deviceModel.at(i) -> property("ID") == id)
        {
            m_deviceModel.removeAt(i);
            emit modelChanged();
            qDebug() << "Device with ID:" + QString::number(id) + " disconnected!";
            return true;
        }
    }

    return false;
}
