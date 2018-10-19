#ifndef NETWORKMETHODS_H
#define NETWORKMETHODS_H

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

enum socketStatus
{
    NotWorking,
    Working
};

enum macStatus getMacAddr(char *mac_addr, const char *ip_addr);
enum ipStatus getIPAndIFACE(char *ip_addr, char *iface);
enum socketStatus createAndBindSocket(int *server_socket, const int *port);
enum socketStatus sendConnectionRequest(int *client_socket, const char *dest_ip, const int *port, const char *local_ip, const char *MAC);
void checkMessageBox(int *server_socket, struct Message *msg);
void sendMessage(int *mySocket, struct Message *msg, const int id, const char *ip, const int *port);
int shutdownSocket(const int *socket);

#endif // NETWORKMETHODS_H
