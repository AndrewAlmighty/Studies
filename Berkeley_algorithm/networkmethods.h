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

enum macStatus
{
    CannotGetMacAddr,
    MacAddrFound
};

enum serverStatus
{
    NotRunning,
    Running
};

enum MessageBoxStatus
{
    MessageBoxEmpty,
    MessageBoxNotEmpty
};

enum MessageTypes
{
    HelloMsg,   //Client "hello". Request to join.
    WelcomeMsg, //Server "hello". Accept the client. Provides ID.
    GoodByeMsg  //This message means that someone shutdown the connection or Server refuse the client to connect.
};

enum connectionStatus
{
    cannotConnect,
    serverRefuse,
    connected
};

enum serverStatus createUDPServer(int *server_socket, const int port);
enum MessageBoxStatus checkMessageBox(int *server_socket, const int port);
enum connectionStatus connectToServer(int *client_socket, const char *ip, const int port);
int shutdownSocket(const int *socket);
enum macStatus getMacAddr(char *mac_addr, const char *ip_addr);
enum ipStatus getIPAndIFACE(char *ip_addr, char *iface);

#ifdef __cplusplus
}
#endif

#endif // NETWORKMETHODS_H
