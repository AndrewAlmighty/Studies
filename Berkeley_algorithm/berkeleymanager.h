/* This is main manager. Here are requests to do all network stuff like start working as server/client, detect devices, set time etc.
 * Manager contains:
 * - Clock - it's responsible for internal clock.
 * - NetwormManager - it's responsible for managing network
 */

#ifndef BERKELEYMANAGER_H
#define BERKELEYMANAGER_H

#include <string>
#include <memory>
#include <list>

#include "networkmanager.h"
#include "clock.h"
#include "device.h"
#include "message.h"

class BerkeleyManager
{
public:
    BerkeleyManager();

    bool PrepareToRunAsServer(int port);
    bool PrepareToRunAsClient(std::string ip, int port);
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
    bool handleMessage(const struct Message *msg);
    void runAsServer();
    void runAsClient();
    bool makingConnection(struct Message *msg);
    std::unique_ptr<NetworkManager> m_network;
    std::unique_ptr<Clock> m_clock;
};

#endif // BERKELEYMANAGER_H
