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
    void setIPandIfc(std::string s);
    void setMac(std::string mac);
    void setID(int id);
    void setMode(Mode m);

    int getID() const;
    std::string getIP() const;
    std::string getMAC() const;
    std::string getMode() const;
    std::string getInterface() const;

private:
    Mode m_mode;
    std::string m_ip;
    std::string m_ifc;
    std::string m_mac;
    int m_ID;
};

#endif // DEVICE_H
