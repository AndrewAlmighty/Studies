#ifndef BERKELEYMANAGER_H
#define BERKELEYMANAGER_H

#include <string>
#include <memory>
#include <list>

#include "networkmanager.h"
#include "clock.h"
#include "device.h"

class BerkeleyManager
{
public:
    BerkeleyManager();

    bool PrepareToRunAsServer();
    bool PrepareToRunAsClient(std::string ip);
    void DetectServers();
    void start();
    void Stop();

    void setTime(std::string time);
    int getID() const;
    std::string getMode() const;
    std::string getIP() const;
    std::string getMAC() const;
    std::string getTime() const;

private:
    Device m_device;
    std::unique_ptr<NetworkManager> m_network;
    std::unique_ptr<Clock> m_clock;
};

#endif // BERKELEYMANAGER_H
