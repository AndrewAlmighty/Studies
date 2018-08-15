#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <list>
#include "device.h"
#include "message.h"

class NetworkManager
{
public:
    NetworkManager();

    bool createServer(int *port);
    bool connectTo(std::string ip, const int &port);
    bool shutdownConnection();
    bool handleConnectionRequest(const Message *msg);
    void checkMailBox(Message *msg);
    void sendMsg(const Message *msg, const std::string &ip);
    void resetIDCounter();
    Device getDevice() const;

private:
    void acceptClient(const std::string &ip, const std::string &mac);
    void addDeviceToNetworkList(const std::string &ip, const std::string &mac, const int id, Device::Mode mode);
    std::string readMac(std::string ifc);
    std::string readIPandIfc();

    int m_socket;
    int m_IDcounter;  //this value sets id of every device. It has to be unique.
    int m_port;       //on which port we work.
    Device m_device;
    std::list <Device> m_deviceList;
};

#endif // NETWORKMANAGER_H
