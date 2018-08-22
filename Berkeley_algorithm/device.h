/* The Device class. Here we keep information about device.
 */

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
    Device(const Device &dev);
    bool operator==(const Device &dev);

    void setIPandIfc(std::string s);
    void setIP(std::string ip);
    void setMac(std::string mac);
    void setID(int id);
    void setMode(Mode m);
    void setModeFromString(std::string mode);
    void resetDevice();
    void setReady(bool isReady);

    int getID() const;
    std::string getIP() const;
    std::string getMAC() const;
    std::string getModeStr() const;
    Device::Mode getMode() const;
    std::string getInterface() const;
    bool isReady() const;

private:
    enum Mode m_mode;
    std::string m_ip;
    std::string m_ifc;
    std::string m_mac;
    bool m_ready;
    int m_ID;
};

#endif // DEVICE_H
