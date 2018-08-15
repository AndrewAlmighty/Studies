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
}

void GuiManager::restartConfiguration()
{
    //Restart method. Do it before you click run.
    m_mode = "Server";
    m_time = "Not Specified";
    m_ip = "Not Specified";
    m_mac = "Not Specified";
    m_ID = -1;
}

void GuiManager::beginJob()
{
    /* This method runs after run is clicked. First we turn off detecting servers, if this option is on.
     * Then, we send information to our manager to start working as Server or Client.
     * If everything is ok and program works as Server/Client, we get all needed information for Gui and change running flag.
     */
    setDetectServers(false);

    if(m_mode == "Server")
    {
        qDebug() << "Prepare to run as Server!";
        if(m_berkeley -> PrepareToRunAsServer(port().toInt()) == false)
            return;
    }

    else if(m_mode == "Client")
    {
        qDebug() << "Prepare to run as Client!";
        if(m_berkeley -> PrepareToRunAsClient(m_serverIP.toStdString(), port().toInt()) == false)
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
    //Stop the job. We do it when abort is clicked.
    if(m_berkeley -> Stop() == false)
        qDebug() << "Failed to close connection";

    restartConfiguration();
    setRunning(false);
}

void GuiManager::changeClock(QString time)
{
    //This function manually changes clock in our device to time which we set in gui.
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

void GuiManager::setDetectServers(bool detect)
{
    if(m_detectingServers == detect)
        return;

    m_detectingServers = detect;

    if(m_detectingServers == true)
        m_berkeley -> DetectServers();

    emit detectServersChanged();
    qDebug() << "Detecting servers: " << m_detectingServers;
}

bool GuiManager::detectServers() const
{
    return m_detectingServers;
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

void GuiManager::setPort(QString port)
{
    if(m_port == port)
        return;

    m_port = port;
    emit portChanged();
    qDebug() << "PORT:" << m_port;
}

QString GuiManager::port() const
{
    return m_port;
}

void GuiManager::setRunning(bool running)
{
    //Setting running flag. If true, get actual time here to have good time on clock!
    if(m_running == running)
        return;

    m_running = running;
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
    //This method adds device to DeviceModel.

    m_deviceModel.append(new DeviceModel(id, ip, mac, mode));
    emit modelChanged();
    qDebug() << "New device connected! ID: " + QString::number(id) + " IP:" + ip + " MAC:" + mac + " Mode:" + mode;
}

bool GuiManager::removeDevice(int id)
{
    //This method removes device from DeviceModel

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

void GuiManager::removeAllDevices()
{
    //This method clears list of devices in DeviceModel
    m_deviceModel.clear();
}
