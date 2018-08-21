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

    bool prepareToRunAsServer(int port);
    bool prepareToRunAsClient(std::string ip, int port);
    void detectServers();
    void start();
    bool stop();

    void setCheckTime(const int time);
    void setTime(std::string time);
    int getID() const;
    std::string getMode() const;
    std::string getIP() const;
    std::string getMAC() const;
    std::string getTime() const;

private:
    enum updateListAction
    {
        addDevice,
        removeDevice,
        clearList
    };

    bool handleMessage(struct Message *msg);
    bool makingConnection(struct Message *msg);
    void runAsServer();
    void runAsClient();
    void updateGui(std::string status);
    void updateDevicesList(updateListAction action, const Device &dev);
    void setGuiDevicesList();
    void breakAll();

    std::unique_ptr<NetworkManager> m_network;
    std::unique_ptr<Clock> m_clock;
    int m_checkTime;                    //check time every x seconds
};

#endif // BERKELEYMANAGER_H
