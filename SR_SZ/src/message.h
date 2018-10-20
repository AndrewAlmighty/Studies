#ifndef MESSAGE_H
#define MESSAGE_H

#define MESSAGE_MAX_LENGTH 16

enum MessageType
{
    //This is message type. It's fast recognize what says that message.

    EmptyMessage,           //Empty message - just ignore it.
    ConnectionRequest,      //Ask for connection
    ConnectionAccepted,     //Connection accepted
    Election,               //Let's make the leader election.
    CheckConnection,        //Leader calls for checking connection.
    ConnectionsConfirm,     //Process confirms that he is online
    AddProcess,             //Add process to list
    RemoveProcess           //Remove process from the list
};

struct Message
{
    enum MessageType type;
    int sender_id;
    int original_sender;
    char message[MESSAGE_MAX_LENGTH];       //We will not send message longer than ipv4 - xxx.xxx.xxx.xxx
};

#endif // MESSAGE_H
