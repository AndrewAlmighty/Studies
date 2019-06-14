#include "networkmethods.h"

#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

bool createAndBindSocket(int *server_socket, const unsigned *port)
{
    //create socket
    if((*server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "Socket creation failed! Errno: %d\n", errno);
        return false;
    }

    //fill information about server. Bind.
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(*port);

    if(bind(*server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        fprintf(stderr, "Bind failed! Errno:%d\n", errno);
        return false;
    }

    return true;
}

void sendConnectionRequest(int *client_socket, const char *dest_ip, const unsigned *port, struct Message *msg)
{
    sendMessage(client_socket, msg, msg -> sender_port, dest_ip, port);
}

void checkMessageBox(const int *socket, struct Message *msg)
{
    //check if we have messages in our messagebox.
    struct sockaddr_in client_addr;
    unsigned int socket_len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    recvfrom(*socket, msg, sizeof(*msg), MSG_DONTWAIT, (struct sockaddr *) &client_addr, &socket_len);

    if(msg -> type == ConnectionRequest)
        strcpy(msg -> text, inet_ntoa(client_addr.sin_addr));
}

void sendMessage(const int *mySocket, struct Message *msg, const unsigned sender_port, const char *ip, const unsigned *port)
{
    fprintf(stderr, "sending to %s:%u, type:%d\n", ip, *port, msg->type);
    msg -> sender_port = sender_port;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(*port);
    sendto(*mySocket, msg, sizeof(*msg), MSG_CONFIRM, (const struct sockaddr *)&addr,sizeof(addr));
}

bool shutdownSocket(const int *socket)
{
    if(close(*socket) < 0)
    {
        fprintf(stderr, "Cannot close the socket! Errno: %d\n", errno);
        return false;
    }

    fprintf(stderr, "Socket is closed!\n");
    return true;
}
