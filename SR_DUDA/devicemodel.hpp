#ifndef DEVICEMODEL_HPP
#define DEVICEMODEL_HPP

#include <QObject>

class DeviceModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString IP READ IP WRITE setIP NOTIFY IPChanged)
    Q_PROPERTY(QString timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)

public:
    explicit DeviceModel(QString ip, unsigned port, QString timestamp = "", QObject *parent = nullptr);
    QString IP() const;
    unsigned port() const;
    QString timestamp() const;

    void setIP(QString ip);
    void setPort(unsigned port);
    void setTimestamp(QString timestamp);

signals:
    void portChanged();
    void IPChanged();
    void timestampChanged();

private:
    QString m_ip = "";
    unsigned m_port = 0;
    QString m_timestamp = "";
};

#endif // DEVICEMODEL_HPP
