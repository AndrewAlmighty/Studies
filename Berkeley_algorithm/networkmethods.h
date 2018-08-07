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
void getMacAddr();
enum ipStatus getIpAddr(char *ip_addr);

#ifdef __cplusplus
}
#endif

#endif // NETWORKMETHODS_H
