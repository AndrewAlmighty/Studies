#include "guicontroller.hpp"

GuiController &GuiController::GetInstance()
{
    static GuiController instance;
    return instance;
}

GuiController::GuiController(QObject *parent)
{
}


void GuiController::getIPandPort(QString &ip, unsigned &port)
{
    QString tmp;
    bool gotPort = false;

    for (int i = 0; i < ip.length(); i++)
    {
        if (!gotPort && ip[i] != ":")
            tmp += ip[i];

        else if (!gotPort && ip[i] == ":")
            gotPort = true;

        else if (gotPort)
        {
            port = ip.mid(i, ip.length() - i).toUInt();
            break;
        }
    }

    ip = tmp;
}

void GuiController::setStatus(const QString &status)
{
    if (status == m_Status)
        return;

    m_Status = status;
    emit statusChanged();
}

QString GuiController::status() const
{
    return m_Status;
}

void GuiController::setInCritical(const bool status)
{
    if (m_InCritical == status)
        return;

    m_InCritical = status;
    emit inCriticalChanged();
    m_BtnEnable = true;
    emit btnEnableChanged();
}

bool GuiController::inCritical() const
{
    return m_InCritical;
}

void GuiController::btnClicked(bool x)
{
    if (!m_BtnEnable)
        return;

    if (!m_InCritical)
    {
        m_BtnEnable = false;
        emit btnEnableChanged();
        m_Controller.enterToCriticalSection();
    }

    else
        m_Controller.leaveCriticalSection();
}

bool GuiController::btnEnable() const
{
    return m_BtnEnable;
}

void GuiController::connectTo(const QString &ip)
{
    if (!m_Controller.isReady())
        return;

    unsigned port = 0;
    QString just_ip = ip;
    getIPandPort(just_ip, port);
    setStatus("Connecting to " + just_ip + " ...");
    if (m_Controller.connectTo(just_ip.toStdString(), port))
        setStatus("Ready to enter into critical section");

    else
        setStatus("Ready to connect");
}

void GuiController::setOurIp(const QString &ip)
{
    if(m_ourIP == ip)
        return;

    reset();
    m_ourIP = ip;
    emit ourIpChanged();

    unsigned port = 0;
    QString just_ip = ip;
    getIPandPort(just_ip, port);
    setStatus("Connecting to" + just_ip + " ...");
    if (m_Controller.setNetworking(just_ip.toStdString(), port))
    {
       m_NetworkGood = true;
       emit networkGoodChanged();
       setStatus("Ready to connect");
       m_BtnEnable = true;
       emit btnEnableChanged();
    }

    else
        setStatus("Not ready to work.");
}

QString GuiController::ourIp() const
{
    return m_ourIP;
}

bool GuiController::networkGood() const
{
    return m_NetworkGood;
}

void GuiController::reset()
{
    m_Status = "Not ready to work.";
    m_InCritical = false;
    m_BtnEnable = false;
    m_NetworkGood = false;
    emit networkGoodChanged();
    emit btnEnableChanged();
    emit inCriticalChanged();
    emit statusChanged();
    m_Controller.reset();
}


