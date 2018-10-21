#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#include "miscellaneous_methods.h"
#include "stdio.h"

bool check_ip(const char* ip_from_arg, char** ip)
{
    unsigned i, k = 0, l = 0, len = strlen(ip_from_arg);   //k is used to count numbers between '.', l is used to count '.'.
    bool ip_is_bad = false;

    for (i = 0; i < len; i++)
    {
        //if char is not '.' or digit, break.
        if(*(ip_from_arg + i) != '.' && !isdigit(*(ip_from_arg + i)))
        {
            ip_is_bad = true;
            break;
        }

        //If it's number, inc k. Then check we got more than 3 numbers between '.'.
        if (isdigit(*(ip_from_arg + i)))
        {
            k++;
            if (k > 3)
            {
                ip_is_bad = true;
                break;
            }

            continue;
        }

        //if we got '..', to many '.' or '.' is on the end.
        if ((k == 0 || l > 3 || i == len - 1) && *(ip_from_arg + i) == '.')
        {
            ip_is_bad = true;
            break;
        }

        //if it's '.', inc l and k = 0.
        if (*(ip_from_arg + i) == '.')
        {
            k = 0;
            l++;
        }
    }

    if(ip_is_bad)
        return false;

    *ip = (char*) malloc(sizeof(char) * len);
    strcpy(*ip, ip_from_arg);
    return true;
}

bool check_port(const char *arg_port, unsigned *port)
{
    int tmp = atoi(arg_port);

    if(tmp <= 0)
        return false;

    *port = tmp;
    return true;
}

bool check_time(const char *arg_time, unsigned *time)
{
    int tmp = atoi(arg_time);

    if (tmp < 1)
        return false;

    *time = tmp;
    return true;
}

void convert_int_to_string(char* str, int integer)
{
    sprintf(str, "%d", integer);
}

void get_str_of_message_type(char *str, const int type)
{
    switch (type)
    {
    case 1:
        strcpy(str, "EmptyMessage");
        break;

    case 2:
        strcpy(str, "ConnectionRequest");
        break;

    case 3:
        strcpy(str, "ConnectionAccepted");
        break;

    case 4:
        strcpy(str, "Election");
        break;

    case 5:
        strcpy(str, "CheckConnection");
        break;

    case 6:
        strcpy(str, "ConnectionsConfirm");
        break;

    case 7:
        strcpy(str, "AddProcess");
        break;

    case 8:
        strcpy(str, "RemoveProcess");
        break;

    case 9:
        strcpy(str, "RequestRingInfo");
        break;

    case 10:
        strcpy(str, "SomeRingInfo");
        break;

    default:
        strcpy(str, "Unknown message type");
        break;
    }
}

void print_added_new_process(const char *ip)
{
    printf("!-> New process joined the ring! It's adress is:%s\n", ip);
}

void print_allocate_failed()
{
    printf("!!!-> Error! Cannot allocate memory! Errno:%d\n", errno);
}

void print_help()
{
    printf("Example of Tanenbaum algorithm. Usage:\n"
           "-ip <ip>        -- provide an ip address to which we want to connect\n"
           "-port <port>    -- provide a port to which we want to connect. Value has to be bigger than 0. Default is 9000\n"
           "-start          -- this is first process in topology, let's start and wait for others\n"
           "-time <sec>     -- set the time of frequency of checking connection\n");
}

void print_message_should_not_be_handled()
{
    printf("!!!-> Warning! There was a message which shouldn't be handled in message handler!\n");
}

void print_process_works(unsigned port, unsigned id)
{
    printf("!-> Tanenbaum algorithm process just started working! Process ID:%u. Port:%u\n", id, port);
}

void print_received_message_from(const unsigned id, const char *ip, const int type)
{
    char msgType[20];
    get_str_of_message_type(msgType, type);
    printf("!-> Received message from process with ID:%u. It's IP is:%s. Type of message:%s\n", id, ip, msgType);
}

void print_remove_process(const unsigned *id, const char *ip)
{
    printf("!-> Process was removed from ring! It's ID is:%u, it's ip adress is:%s\n", *id, ip);
}

void print_sending_message_to(const unsigned id, const char *ip, const int type)
{
    char msgType[20];
    get_str_of_message_type(msgType, type);
    printf("!-> Message has been send to process with ID:%u. It's IP is:%s. Type of message:%s\n", id, ip, msgType);
}
