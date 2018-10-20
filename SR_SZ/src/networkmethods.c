#include "networkmethods.h"

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

enum socketStatus createAndBindSocket(int *server_socket, const unsigned *port)
{
    //create socket
    if((*server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "Socket creation failed! Errno: %d\n", errno);
        return NotWorking;
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
        return NotWorking;
    }

    return Working;
}

enum socketStatus sendConnectionRequest(int *client_socket, const char *dest_ip, const unsigned *port)
{
    if(createAndBindSocket(client_socket, port) == NotWorking)
        return NotWorking;

    //Prepare message for connection request.
    struct Message msg;
    msg.type = ConnectionRequest;

    sendMessage(client_socket, &msg, -1, dest_ip, port);
    return Working;
}

void checkMessageBox(const int *socket, struct Message *msg)
{
    //check if we have messages in our messagebox.
    struct sockaddr_in client_addr;
    unsigned int socket_len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    recvfrom(*socket, msg, sizeof(*msg), MSG_DONTWAIT, (struct sockaddr *) &client_addr, &socket_len);

    if(msg -> type == ConnectionRequest)
        strcpy(msg -> ip, inet_ntoa(client_addr.sin_addr));
}

void sendMessage(const int *mySocket, struct Message *msg, const int id, const char *ip, const unsigned *port)
{
    msg -> sender_id = id;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(*port);
    sendto(*mySocket, msg, sizeof(*msg), MSG_CONFIRM, (const struct sockaddr *)&addr,sizeof(addr));
}

int shutdownSocket(const int *socket)
{
    //Shutdown the socket. If it closed, return 1. Else return 0.
    if(close(*socket) < 0)
    {
        fprintf(stderr, "Cannot close the socket! Errno: %d\n", errno);
        return 0;
    }

    fprintf(stderr, "Socket is closed!\n");
    return 1;
}
