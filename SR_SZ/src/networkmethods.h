#ifndef NETWORKMETHODS_H
#define NETWORKMETHODS_H

#include "message.h"

enum socketStatus
{
    NotWorking,
    Working
};

enum socketStatus createAndBindSocket(int *server_socket, const unsigned *port);
enum socketStatus sendConnectionRequest(int *client_socket, const char *dest_ip, const unsigned *port);
void checkMessageBox(int *socket, struct Message *msg);
void sendMessage(int *mySocket, struct Message *msg, const int id, const char *ip, const unsigned *port);
int shutdownSocket(const int *socket);

#endif // NETWORKMETHODS_H
