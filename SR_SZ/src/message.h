/*
 * Here we have defined message structure, which one is distributed throughout the network.
 * Message contains:
 * MessageType,
 * ID - used to recognize who is sending the message
 * message - contains a message to read.
 */

#ifndef MESSAGE_H
#define MESSAGE_H

enum MessageType
{
    //This is message type. It's fast recognize what says that message.

    EmptyMessage,           //Empty message - just ignore it.
    ConnectionRequest,      //client is asking to join the network.
    ConnectionAccepted,     //Server accept client.
    ConnectionRefused,      //server refuse client's connection request.
    ClientDisconnect,       //client is leaving the network.
    ServerBrokeConnection,  //server tells client that he is disconnect.
    ClientReady,            //client is prepared to work
    NetworkSizeRequest,     //Request for size of network
    NetworkSize,            //Info with size of network
    DeviceInfoRequest,      //Request for info about Device with ID
    DeviceInfo,             //Send info about Device
    ClientCheckRequest,     //Server checks if clients are connected.
    ClientConfirm,          //Client confirm that he is connected.
    ClientTime,             //client sends his time.
    TimeRequest,            //server ask clients to send him time.
    TimeAdjustRequest,      //server ask clients to adjust time.
};

struct Message
{
    enum MessageType type;
    int sender_id;
    char message[128];
};

#endif // MESSAGE_H
