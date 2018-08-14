#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include "device.h"
#include "message.h"

class NetworkManager
{
public:
    NetworkManager(Device &dev);

    bool createServer(int port);
    bool connectTo(std::string ip, int port, int *id);
    bool shutdownConnection();
    void checkMailBox(struct Message *msg);

private:
    int m_socket;
    std::string readMac(std::string ifc);
    std::string readIPandIfc();
};

#endif // NETWORKMANAGER_H
