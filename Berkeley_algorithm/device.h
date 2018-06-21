#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class Device
{
public:
    enum Mode{
        NotSpecified,
        Server,
        Client
    };

    Device();
    void setIp(std::string ip);
    void setMac(std::string mac);
    void setID(int id);
    void setMode(Mode m);

    int getID() const;
    std::string getIP() const;
    std::string getMAC() const;
    std::string getMode() const;

private:
    Mode m_mode;
    std::string m_ip;
    std::string m_mac;
    int m_ID;
};

#endif // DEVICE_H
