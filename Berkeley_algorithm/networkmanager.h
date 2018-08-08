#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include "device.h"

class NetworkManager
{
public:
    NetworkManager(Device &dev);

    bool createServer(int port);
    bool connectTo(std::string ip, int port);
    bool shutdownConnection();
    bool checkMailBox();

private:
    int m_socket;
    std::string readMac(std::string ifc);
    std::string readIPandIfc();
};

#endif // NETWORKMANAGER_H
