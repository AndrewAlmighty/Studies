#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include "device.h"

class NetworkManager
{
public:
    NetworkManager(Device &dev);

    bool createServer(int port);
    bool shutdownConnection();
    void listen();

private:
    int m_socket;
    std::string readMac(std::string ifc);
    std::string readIPandIfc();
};

#endif // NETWORKMANAGER_H
