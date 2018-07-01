#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>

class DeviceModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int ID READ ID WRITE setID NOTIFY IDChanged)
    Q_PROPERTY(QString IP READ IP WRITE setIP NOTIFY IPChanged)
    Q_PROPERTY(QString MAC READ MAC WRITE setMAC NOTIFY MACChanged)
    Q_PROPERTY(QString Mode READ Mode WRITE setMode NOTIFY ModeChanged)

public:
    explicit DeviceModel(int id, QString ip, QString mac, QString mode, QObject *parent = nullptr);

    int ID() const;
    QString IP() const;
    QString MAC() const;
    QString Mode() const;

    void setID(int id);
    void setIP(QString ip);
    void setMAC(QString mac);
    void setMode(QString mode);

signals:
    void IDChanged();
    void IPChanged();
    void MACChanged();
    void ModeChanged();

private:
    int m_id;
    QString m_ip;
    QString m_mac;
    QString m_mode;
};

#endif // DEVICEMODEL_H
