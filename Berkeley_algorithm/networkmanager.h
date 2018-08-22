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
    bool handleConnectionRequest(const struct Message *msg, Device &dev);
    bool handleConnectionAcceptedMessage(const struct Message *msg);
    void handleConnectionRefuseMessage();
    void handleNetworkSizeRequest(struct Message *msg);
    bool getDevices(struct Message *msg, const int& size);
    int handleNetworkSize(struct Message *msg);
    void handleTimeRequest(struct Message *msg);
    void handleDeviceInfoRequest(struct Message *msg);
    void checkMailBox(struct Message *msg);
    void sendMsg(struct Message *msg, const std::string &ip = "");
    void resetDevicesList();
    void resetServerIP(const std::string ip = "");
    Device getDevice() const;
    bool getDevicesList(std::list<Device>::const_iterator &it, const int &i) const;

private:
    enum listAction
    {
        addDeviceToList,
        removeDeviceFromList,
        getDeviceFromList,
        clearList
    };

    void acceptClient(Device &dev, const std::string &ip, const std::string &mac);
    void actionOnNetworkDevicesList(listAction action, const int &id, Device *dev = nullptr, const std::string &ip = "", const std::string &mac = "",  const Device::Mode mode = Device::NotSpecified);
    std::string getIpFromList(const int &id);
    std::string readMac(std::string ifc);
    std::string readIPandIfc();
    std::string getDeviceIp(const int &id) const;

    int m_socket;
    int m_IDcounter;  //this value sets id of every device. It has to be unique.
    int m_port;       //on which port we work.
    std::string m_server_ip;
    Device m_device;
    std::list <Device> m_deviceList;
};

#endif // NETWORKMANAGER_H
