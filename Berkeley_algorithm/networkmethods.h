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

int runUDPServer();
int connectTo();
void disconnectFrom();
void sendMessage();

#ifdef __cplusplus
}
#endif

#endif // NETWORKMETHODS_H
