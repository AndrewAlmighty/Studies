#ifndef TANENBAUM_H_
#define TANENBAUM_H_

#include "miscellaneous_methods.h"
#include "networkmethods.h"

struct RingInfo
{
    unsigned process_id;            //id of this process.
    unsigned id_counter;            //This varriable sets id of every process.
    unsigned process_counter;       //counts how many processes are in the ring.
    int socket;                     //socket which we use for communication.
    unsigned port;                  //port which we use for communication.
    unsigned checkConnection_time;  //check connection every [checkConnection_time] seconds.
    unsigned checkLeader_time;      //process will check leader every [checkLeader_time] seconds.
    unsigned *id_arr;               //keeps IDs of processes in the ring
    int leader_id;                  //keeps leader's id. If -1, we don't know who is he.
    bool is_leader;                 //is this process is a leader
    char tmp_ip[16];                //here we keep ip for short time. We avoid to create other pointers.
    char *ip_arr;                   //keeps ips of processes. Format of this array is: <ip>;<ip>;
                                    //this process ip in this array is marked as 127.0.0.1
} ring_info;

//this method adds a new process to RingInfo structure.
bool add_new_process_to_ring(const char* ip);

//this method ask process to which we connect to send us IDs and IPs of other processes which
//are also in the same ring.
void call_for_info_about_other_processes(const char* ip, const unsigned node_id);

//Check if in the ring are at least two processes. In other case, terminate.
bool are_enough_process_to_continue();

//Find ip of process with a specific identifier. IP is null when we don't find it.
void find_ip(unsigned id, char* ip);

//Sometimes we need to know which place in id_arr has our process.
int get_idx_from_id_arr(unsigned id);

//Handle incoming message
void handle_message(struct Message *msg, bool is_ready);

// --------------- Message handlers ---------------------------

bool handle_AddProcess(struct Message *msg);
bool handle_CheckConnection(struct Message *msg);
bool handle_ConnectionRequest(struct Message *msg);
bool handle_RemoveProcess(struct Message *msg);
bool handle_RequestRingInfo(struct Message *msg);
bool handle_SomeRingInfo(struct Message *msg);

//---------------- Message handlers ---------------------------

//This method creates socket and do everything what is needed before we join/create the ring.
bool prepare_process(bool is_start_node, const unsigned time_cc, const unsigned time_cl, const unsigned *port, const char* my_ip, const char *ip);

//clear the ring structure.
void remove_all_processes_from_ring();

//remove process from RingInfo structure.
bool remove_process_from_ring(const unsigned id);

//Main loop. Here algorithm works.
void run();

//If the message has not gone through all processes, send it to next process.
bool send_message_to_next_process(struct Message *msg);

//Wait for x seconds for specific message. Handle others messages meanwhile.
bool wait_for_specific_message(unsigned sec, enum MessageType msgType, struct Message *msg);

#endif
