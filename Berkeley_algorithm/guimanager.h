#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QObject>

class GuiManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString serverIP READ serverIP WRITE setServerIP NOTIFY serverIPChanged)
    Q_PROPERTY(QString MAC READ MAC WRITE setMAC NOTIFY MACChanged)

public:
    static GuiManager & GetInstance();

    Q_INVOKABLE void setTime(QString time);
    QString time() const;

    Q_INVOKABLE void setIp(QString ip);
    QString ip() const;

    Q_INVOKABLE void setMode(QString mode);
    QString mode() const;

    Q_INVOKABLE void setServerIP(QString ip);
    QString serverIP() const;

    Q_INVOKABLE void setMAC(QString mac);
    QString MAC() const;

signals:
    void timeChanged();
    void ipChanged();
    void modeChanged();
    void serverIPChanged();
    void MACChanged();

private:
    GuiManager(QObject *parent = nullptr);
    GuiManager(const GuiManager &);
    GuiManager& operator =(const GuiManager&);

    QString m_time;
    QString m_ip;
    QString m_mode;
    QString m_serverIP;
    QString m_mac;
};

#endif // GUIMANAGER_H
