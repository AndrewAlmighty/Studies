#ifndef NETWORKMETHODS_H
#define NETWORKMETHODS_H

#ifdef __cplusplus
extern "C"{
#endif

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
int turnOffSocket(int *socket);

#ifdef __cplusplus
}
#endif

#endif // NETWORKMETHODS_H
