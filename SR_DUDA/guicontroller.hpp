#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP

#include <QObject>
#include "controller.hpp"

class GuiController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString ourIp READ ourIp WRITE setOurIp NOTIFY ourIpChanged)
    Q_PROPERTY(bool networkGood READ networkGood NOTIFY networkGoodChanged)
    Q_PROPERTY(bool inCritical READ inCritical WRITE setInCritical NOTIFY inCriticalChanged)
    Q_PROPERTY(bool btnEnable READ btnEnable WRITE btnClicked NOTIFY btnEnableChanged)

public:
    static GuiController& GetInstance();

    Q_INVOKABLE void setStatus(const QString& status);
    QString status() const;

    Q_INVOKABLE void setInCritical(const bool status);
    bool inCritical() const;

    Q_INVOKABLE void btnClicked(bool x = false);
    bool btnEnable() const;

    Q_INVOKABLE void setOurIp(const QString &ip);
    QString ourIp() const;

    Q_INVOKABLE void connectTo(const QString &ip);

    bool networkGood() const;
    void reset();

signals:
    void statusChanged();
    void inCriticalChanged();
    void ourIpChanged();
    void btnEnableChanged();
    void networkGoodChanged();

private:
    GuiController(QObject *parent = nullptr);
    GuiController(const GuiController&) = delete;
    GuiController& operator=(const GuiController&) = delete;
    void getIPandPort(QString &ip, unsigned &port);

    Controller m_Controller;
    QString m_Status = "Not ready to work.";
    QString m_ourIP;
    bool m_InCritical = false;
    bool m_BtnEnable = false;
    bool m_NetworkGood = false;
};

#endif // GUICONTROLLER_HPP
