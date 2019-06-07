#ifndef MESSAGE_H
#define MESSAGE_H

#define MSG_MAX_LEN 16

enum MessageType
{
    //This is message type. It's fast recognize what says that message.

    EmptyMessage,           //Empty message - just ignore it.
    ConnectionRequest,      //Ask for connection
    ConnectionAccepted,     //Connection accepted
    Disconnect              //disconnect
};

struct Message
{
    enum MessageType type;
    int sender_port;
    char text[MSG_MAX_LEN];
    long int timestamp;
};

#endif // MESSAGE_H
