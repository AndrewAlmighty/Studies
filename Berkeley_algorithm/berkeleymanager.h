/* This is main manager. Here are requests to do all network stuff like start working as server/client, detect devices, set time etc.
 * Manager contains:
 * - object Clock - it's responsible for internal clock.
 * - object Device - it contains information about this device.
 */

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

    bool PrepareToRunAsServer(int port);
    bool PrepareToRunAsClient(std::string ip);
    void DetectServers();
    void start();
    bool Stop();

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
