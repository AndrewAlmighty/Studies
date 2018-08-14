#ifndef NETWORKMETHODS_H
#define NETWORKMETHODS_H

#ifdef __cplusplus
extern "C"{
#endif

#include "message.h"

enum ipStatus
{
    CannotGetIP,
    IPFound,
    MoreThanOneIp
};

enum macStatus
{
    CannotGetMacAddr,
    MacAddrFound
};

enum serverStatus
{
    NotRunning,
    Running
};

enum connectionStatus
{
    CannotConnect,
    ServerRefuse,
    Connected
};

enum serverStatus createUDPServer(int *server_socket, const int port);
void checkMessageBox(int *server_socket, struct Message *msg);
enum connectionStatus connectToServer(int *client_socket, const char *ip, const int port, int* device_id);
int shutdownSocket(const int *socket);
enum macStatus getMacAddr(char *mac_addr, const char *ip_addr);
enum ipStatus getIPAndIFACE(char *ip_addr, char *iface);

#ifdef __cplusplus
}
#endif

#endif // NETWORKMETHODS_H
