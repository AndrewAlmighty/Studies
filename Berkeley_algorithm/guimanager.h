#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QObject>
#include <memory>
#include "berkeleymanager.h"

class GuiManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString serverIP READ serverIP WRITE setServerIP NOTIFY serverIPChanged)
    Q_PROPERTY(QString MAC READ MAC WRITE setMAC NOTIFY MACChanged)
    Q_PROPERTY(int ID READ ID WRITE setID NOTIFY IDChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

public:
    static GuiManager & GetInstance();

    Q_INVOKABLE void beginJob();
    Q_INVOKABLE void finishJob();

    Q_INVOKABLE void setTime(QString time);
    QString time() const;

    void setIp(QString ip);
    QString ip() const;

    Q_INVOKABLE void setMode(QString mode);
    QString mode() const;

    Q_INVOKABLE void setServerIP(QString ip);
    QString serverIP() const;

    void setMAC(QString mac);
    QString MAC() const;

    void setRunning(bool connected);
    bool running() const;

    void setID(int id);
    int ID() const;

signals:
    void timeChanged();
    void ipChanged();
    void modeChanged();
    void serverIPChanged();
    void MACChanged();
    void runningChanged();
    void IDChanged();

private:
    GuiManager(QObject *parent = nullptr);
    GuiManager(const GuiManager &);
    GuiManager& operator =(const GuiManager&);
    void restartConfiguration();

    std::unique_ptr<BerkeleyManager> m_berkeley;

    QString m_time;
    QString m_ip;
    QString m_mode;
    QString m_serverIP;
    QString m_mac;
    bool m_running;
    int m_ID;
};

#endif // GUIMANAGER_H
