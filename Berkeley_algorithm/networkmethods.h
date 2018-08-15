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

enum macStatus getMacAddr(char *mac_addr, const char *ip_addr);
enum ipStatus getIPAndIFACE(char *ip_addr, char *iface);
enum serverStatus createAndBindSocket(int *server_socket, const int *port);
enum connectionStatus connectToServer(int *client_socket, const char *dest_ip, const int *port, const char *local_ip, const char *MAC, int* device_id);
void checkMessageBox(int *server_socket, struct Message *msg);
void sendMessage(int *mySocket, const struct Message *msg, const char *ip, int *port);
int shutdownSocket(const int *socket);


#ifdef __cplusplus
}
#endif

#endif // NETWORKMETHODS_H
