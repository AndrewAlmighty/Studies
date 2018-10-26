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
    AddProcess,             //Add process to list
    RemoveProcess,          //Remove process from the list
    RequestRingInfo,        //Request for list of processes in our ring. Used when we join the ring.
    SomeRingInfo,           //Process send info about one process. Exactly one ID and it's IP.
};

struct Message
{
    enum MessageType type;
    int sender_id;
    int original_sender_id;         //First process which has send message. If its ConnectionAccepted msg,
                                    //we send here id for new process.
    int auxiliary_int;              //this integer is used if we want to send some integer for special reason.
    char text[MESSAGE_MAX_LENGTH];    //We will not send message longer than ip address - xxx.xxx.xxx.xxx
                                    //Sometimes used to send other info than ip address

};

#endif // MESSAGE_H
