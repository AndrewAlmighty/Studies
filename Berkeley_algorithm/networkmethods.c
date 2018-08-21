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

enum socketStatus createAndBindSocket(int *server_socket, const int *port)
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

enum ipStatus getIPAndIFACE(char *ip_addr, char *iface)
{
    //This method read ip address and MAC of our machine. We read the first ip address with is not 127.0.0.1. If We read more or none we return error.
    //If interface name will be longer than 5 characters we will also get error.
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
    for (ifa = ifaddr; ifa != NULL; ifa = ifa -> ifa_next)
    {
        family = ifa -> ifa_addr -> sa_family;

        if (family == AF_INET) {
            s = getnameinfo(ifa -> ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0)
            {
                fprintf(stderr, "GetNameInfo() failed. Errno: %d", errno);
                return CannotGetIP;
            }

            if(addr_number == 0 && strcmp("127.0.0.1", host) != 0)
            {
                //get ip address and interface. Interface is needed to get MAC.
                strcpy(ip_addr, host);
                addr_number += 1;
                fprintf(stderr, "Found IP address: %s\n", ip_addr);
                strcpy(iface, ifa -> ifa_name);
                fprintf(stderr, "Interface: %s\n", iface);
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

enum macStatus getMacAddr(char *mac_addr, const char *ifc)
{
    //Get MAC address. Need Interface name to get it.
    int fd;
    struct ifreq ifr;
    unsigned char *mac;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , ifc, IFNAMSIZ-1);
    ioctl(fd, SIOCGIFHWADDR, &ifr);
    close(fd);
    mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;

    //translate address
    static const char* hex_lookup = "0123456789ABCDEF";

    for (int i = 0 ; i < 6 ; i++)
    {
        *mac_addr++ = hex_lookup[mac[i] >> 4];
        *mac_addr++ = hex_lookup[mac[i] & 0x0F];

        if ((i+1)%16 && i != 5)
            *mac_addr++ = ':';
    }

    *mac_addr = '\0';
    return MacAddrFound;
}

enum socketStatus sendConnectionRequest(int *client_socket, const char *dest_ip, const int *port, const char *local_ip, const char *MAC)
{
    if(createAndBindSocket(client_socket, port) == NotWorking)
        return NotWorking;

    //Prepare message for connection request.
    struct Message msg;
    msg.type = ConnectionRequest;
    strcpy(msg.message, "IP:");
    strcat(msg.message, local_ip);
    strcat(msg.message, "_MAC:");
    strcat(msg.message, MAC);

    sendMessage(client_socket, &msg, -1, dest_ip, port);
    return Working;
}

void checkMessageBox(int *server_socket, struct Message *msg)
{
    //check if we have messages in our messagebox.
    struct sockaddr_in client_addr;
    unsigned int socket_len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    recvfrom(*server_socket, msg, sizeof(*msg), MSG_DONTWAIT, (struct sockaddr *) &client_addr, &socket_len);
}

void sendMessage(int *mySocket, struct Message *msg, const int id, const char *ip, const int *port)
{
    msg -> sender_id = id;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(*port);
    sendto(*mySocket, msg, sizeof(*msg), MSG_CONFIRM, (const struct sockaddr *)&addr,sizeof(addr));
    fprintf(stderr, "\n------------ip:%s\n", ip);
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
