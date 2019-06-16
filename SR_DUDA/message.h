#ifndef MESSAGE_H
#define MESSAGE_H

#define MSG_MAX_LEN 16

enum MessageType
{
    //This is message type. It's fast recognize what says that message.

    EmptyMessage,           //Empty message - just ignore it.
    ConnectionRequest,      //Ask for connection
    ConnectionAccepted,     //Connection accepted
    Disconnect,             //disconnect
    NewDevice,
    CheckConnection,
    ConnectionAck,
    AskToEnterCritical,
    AnswerForAsk,
    LeaveCritical
};

struct Message
{
    enum MessageType type;
    unsigned sender_port;
    char text[MSG_MAX_LEN];
    long int timestamp;
};

#endif // MESSAGE_H
