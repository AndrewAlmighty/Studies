#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>

#include "device.h"

class NetworkManager
{
public:
    NetworkManager(Device &dev);

private:
    std::string readMac();
    std::string readIP();
};

#endif // NETWORKMANAGER_H
