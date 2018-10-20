#ifndef TANENBAUM_H_
#define TANENBAUM_H_

#include "miscellaneous_methods.h"
#include "networkmethods.h"

struct RingInfo
{
    unsigned process_id;        //id of this process.
    unsigned id_counter;        //This varriable sets id of every process.
    unsigned process_counter;   //counts how many processes are in the ring.
    int socket;                 //socket which we use for communication.
    unsigned port;              //port which we use for communication.
    unsigned *id_arr;           //keeps IDs of processes in the ring
    bool is_leader;             //is this process is a leader
    char tmp_ip[16];            //here we keep ip for short time. We avoid to create other pointers.
    char *ip_arr;               //keeps ips of processes. Format of this array is: <ip>;<ip>;
                                //this process ip in this array is marked as 127.0.0.1
} ring_info;

//this method adds a new process to RingInfo structure.
bool add_new_process_to_ring(const char* ip);

//this method ask process to which we connect to send us IDs and IPs of other processes which
//are also in the same ring.
void call_for_info_about_other_processes(const char* ip);

//Find ip of process with a specific identifier. IP is null when we don't find it.
void find_ip(unsigned id, char* ip);

//Handle incoming message
void handle_message(struct Message *msg);

//This method creates socket and do everything what is needed before we join/create the ring.
bool prepare_process(bool is_start_node, const unsigned time, const char *ip, const unsigned *port);

//clear the ring structure.
void remove_all_processes_from_ring();

//remove process from RingInfo structure.
bool remove_process_from_ring(const unsigned id);

//Main loop. Here algorithm works.
void run();

#endif
