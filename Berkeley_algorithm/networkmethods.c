#include "networkmethods.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

enum serverStatus createUDPServer(int *server_socket, int port)
{
    //create socket
    if((*server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "Socket creation failed! Errno: %d\n", errno);
        return NotRunning;
    }
    //fill information about server. Bind.
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(*server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        fprintf(stderr, "Bind failed! Errno:%d\n", errno);
        return NotRunning;
    }

    return Running;
}

int turnOffSocket(int *socket)
{
    //Shutdown the socket. If it closed, return 1. Else return 0.
    if(close(*socket) < 0)
    {
        fprintf(stderr, "Cannot close the socket! Errno: %d", errno);
        return 0;
    }

    fprintf(stderr, "Socket is closed!\n");
    return 1;
}
