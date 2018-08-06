#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include "device.h"

class NetworkManager
{
public:
    NetworkManager(Device &dev);

    bool createServer(int port);
    bool connectToServer(std::string ip);
    bool shutdownConnection();

private:
    int m_socket;
    std::string readMac();
    std::string readIP();
};

#endif // NETWORKMANAGER_H
