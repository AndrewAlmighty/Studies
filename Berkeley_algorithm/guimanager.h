/* This is GuiManager class. It's role is simple - manage Gui which one is written in QML.
 * It's singleton. We don't need more that one object of this class and it's useful to get it anywhere.
 * Do not forget to set pointer to BerkeleyManager object!
 */

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
    void removeAllDevices();

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

    QList<QObject*> m_deviceModel;  //It's model. It's role is to show all available servers or all devices which are connected to server
    QString m_time;                 //Shows time
    QString m_ip;                   //Shows our IP
    QString m_mode;                 //Contain information about mode which we want to run: Server or Client
    QString m_serverIP;             //Contain IP of server which we want to connect
    QString m_mac;                  //shows our MAC
    bool m_running;                 //True if we are not in setup window
    bool m_detectingServers;        //True if we want to detect local servers
    int m_ID;                       //Shows ID of device
};

#endif // GUIMANAGER_H
