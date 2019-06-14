#ifndef NETWORKMETHODS_H
#define NETWORKMETHODS_H

#ifdef __cplusplus
extern "C"{
#endif

#include "message.h"
#include <stdbool.h>

bool createAndBindSocket(int *server_socket, const unsigned *port);
void sendConnectionRequest(int *client_socket, const char *dest_ip, const unsigned *port, struct Message *msg);
void checkMessageBox(const int *socket, struct Message *msg);
void sendMessage(const int *mySocket, struct Message *msg, const unsigned sender_port, const char *ip, const unsigned *port);
bool shutdownSocket(const int *socket);

#ifdef __cplusplus
}
#endif

#endif // NETWORKMETHODS_H
