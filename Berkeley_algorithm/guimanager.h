#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QObject>

#include "berkeleymanager.h"
#include "devicemodel.h"

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
    Q_PROPERTY(bool detectServers READ detectServers WRITE setDetectServers NOTIFY detectServersChanged)
    Q_PROPERTY(QList<QObject*> model READ model NOTIFY modelChanged)

public:
    static GuiManager & GetInstance();
    void setManager(BerkeleyManager &manager);

    Q_INVOKABLE void beginJob();
    Q_INVOKABLE void finishJob();
    Q_INVOKABLE void changeClock(QString time);

    Q_INVOKABLE void setTime(QString time);
    QString time() const;

    void setIp(QString ip);
    QString ip() const;

    Q_INVOKABLE void setMode(QString mode);
    QString mode() const;

    Q_INVOKABLE void setDetectServers(bool detect);
    bool detectServers() const;

    Q_INVOKABLE void setServerIP(QString ip);
    QString serverIP() const;

    void setMAC(QString mac);
    QString MAC() const;

    void setRunning(bool connected);
    bool running() const;

    void setID(int id);
    int ID() const;

    QList<QObject*> model() const;
    void addDevice(int id, QString ip, QString mac, QString mode);
    bool removeDevice(int id);

signals:
    void timeChanged();
    void ipChanged();
    void modeChanged();
    void serverIPChanged();
    void MACChanged();
    void runningChanged();
    void IDChanged();
    void modelChanged();
    void detectServersChanged();

private:
    GuiManager(QObject *parent = nullptr);
    GuiManager(const GuiManager &);
    GuiManager& operator =(const GuiManager&);
    void restartConfiguration();

    BerkeleyManager* m_berkeley;    //pointer to main manager. Do not destroy!

    QList<QObject*> m_deviceModel;
    QString m_time;
    QString m_ip;
    QString m_mode;
    QString m_serverIP;
    QString m_mac;
    bool m_running;
    bool m_detectingServers;
    int m_ID;
};

#endif // GUIMANAGER_H
