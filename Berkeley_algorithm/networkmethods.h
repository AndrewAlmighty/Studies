#ifndef NETWORKMETHODS_H
#define NETWORKMETHODS_H

#ifdef __cplusplus
extern "C"{
#endif

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

enum connectStatus
{
    NotConnected,
    Connected,
    CannotFindServer,
};

enum serverStatus
{
    NotRunning,
    Running
};

enum serverStatus createUDPServer(int *server_socket, int port);
int shutdownSocket(int *socket);
enum macStatus getMacAddr(char *mac_addr, const char *ip_addr);
enum ipStatus getIPAndMAC(char *ip_addr, char *iface);

#ifdef __cplusplus
}
#endif

#endif // NETWORKMETHODS_H
