#include "networkmethods.h"

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
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

int shutdownSocket(int *socket)
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

void getMacAddr()
{

}

enum ipStatus getIpAddr(char *ip_addr)
{
    //This method read ip address of our machine. We read the first ip address with is not 127.0.0.1. If We read more or none we return error.
    struct ifaddrs *ifaddr, *ifa;
    int family, s, addr_number = 0;
    char host[NI_MAXHOST];
    //Get list of network interfaces and ip addresses. Something like ifconfig.
    if (getifaddrs(&ifaddr) == -1)
    {
        fprintf(stderr, "Cannot get list of ip interfaces! Errno: %d", errno);
        return CannotGetIP;
    }

    //Iterate over all founded addresses and found the right one
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0)
            {
                fprintf(stderr, "GetNameInfo() failed. Errno: %d", errno);
                return CannotGetIP;
            }

            if(addr_number == 0 && strcmp("127.0.0.1", host) != 0)
            {
                strcpy(ip_addr, host);
                addr_number += 1;
                fprintf(stderr, "Found ip address: %s\n", ip_addr);
                continue;
            }

            if(addr_number != 0 && strcmp("127.0.0.1", host) != 0)
            {
                fprintf(stderr, "Found more than one ip address!\n");
                return MoreThanOneIp;
            }
        }
    }

    return IPFound;
}
