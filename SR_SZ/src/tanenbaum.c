#include <stdlib.h>
#include <string.h>

#include "networkmethods.h"
#include "tanenbaum.h"

bool add_new_process_to_ring(const char* ip)
{
    //inc the count of processes in the ring.
    ring_info.process_counter += 1;
    //raise the memory reserved by ptr for ids array and add new ID to array.
    ring_info.id_arr = (unsigned*) realloc(ring_info.id_arr, ring_info.process_counter * sizeof (unsigned));

    if (ring_info.id_arr == NULL)
    {
        print_allocate_failed();
        return false;
    }

    ring_info.id_arr[ring_info.process_counter - 1] = ring_info.process_counter - 1;
    //raise the memory reserver by ptr for ips array and add new ip to array.
    ring_info.ip_arr = (char*) realloc(ring_info.ip_arr, ring_info.process_counter * sizeof (char) * MESSAGE_MAX_LENGTH);

    if (ring_info.ip_arr == NULL)
    {
        print_allocate_failed();
        return false;
    }

    strcat(ring_info.ip_arr, ip);
    strcat(ring_info.ip_arr, ";");

    return true;
}

void find_ip(unsigned id, char* ip)
{
    unsigned i, j = 0, counter = 0; //counter counts how many ; we got.
    for (i = 0; ring_info.ip_arr[i] != '\0'; i++)
    {
       if (ring_info.ip_arr[i] == ';')
            counter += 1;

       if (id == counter)
       {
           ip[j] = ring_info.ip_arr[i];
           j += 1;
       }

       else if (counter > id)
           break;
    }
}

void prepare_process(bool is_start_node, const unsigned time, const char *ip, const unsigned port)
{
    //At start we don't have any process in the ring.
    ring_info.process_counter = 0;

    //1.Create a socket
    int socket;
    if (createAndBindSocket(&socket, &port) == NotWorking)
        return;

    //2. Create a message which will handle the first message.
    struct Message msg;
    msg.type = EmptyMessage;

    /* 3a. If it's start mode and we wait for another process to connect and create a ring,
     * wait for other process to connect. This process became a leader. It's ID is 0.
     */
    if (is_start_node == true)
    {
        ring_info.is_leader = true;
        ring_info.process_id = 0;
        add_new_process_to_ring("127.0.0.1");

        while(1)
        {
            checkMessageBox(&socket, &msg);

            if(msg.type == ConnectionRequest)
            {
                break;
            }
        }
    }

    /* 3b. If it's not a start mode, we send request for conenction to other process.
     * When we got ConnectionAccepted message, we have joined the ring.
     */
    else
    {
        ring_info.is_leader = false;
        sendConnectionRequest(&socket, ip, &port);

        while(1)
        {
            checkMessageBox(&socket, &msg);

            if(msg.type == ConnectionAccepted)
            {
                break;
            }
        }
    }

    //4. On the end run process.
    print_process_works(port);
    run();
}

void remove_all_processes_from_ring()
{
    int i;
    for (i = 0; i < ring_info.process_counter; i++)
        remove_process_from_ring(i);
}

bool remove_process_from_ring(const unsigned id)
{
    //If in ring is only one process, free all allocate space and send info that the process has to terminate.
    if (ring_info.process_counter - 1 < 2)
    {
        free(ring_info.id_arr);
        free(ring_info.ip_arr);
        return false;
    }

    //if not, rewrite arrays.
    int i;
    bool found_pos = false;
    for (i = 0; i < ring_info.process_counter; i++)
    {
        if (ring_info.id_arr[i] == id)
            found_pos = true;

        //if id is not on last pos.
        if (found_pos == true && i < ring_info.process_counter - 1)
        {
            ring_info.id_arr[i] = ring_info.id_arr[i + 1];
            if (i == ring_info.process_counter - 2)
                break;
        }
    }

    //find a place where ip to remove starts, when we find it, start rewriting from that pos.
    unsigned counter = 0, start_pos1 = 0, start_pos2 = 0;
    found_pos = false;

    for (i = 0; ring_info.ip_arr[i] != '\0'; i++)
    {
        if (id == counter && found_pos == false)
        {
            start_pos1 = i;
            found_pos = true;
        }

        if (ring_info.ip_arr[i] == ';')
            counter += 1;

        else if (counter > id)
        {
            start_pos2 = i;
            break;
        }
    }

    //if i is pointing to end of string, just remove last ip address.
    if (ring_info.ip_arr[i] == '\0')
    {
        for (i = start_pos1; ring_info.ip_arr[i] != '\0'; i++)
            ring_info.ip_arr[i] = '\0';
    }

    //otherwise, rewrite string and clear it.
    else
    {
        for (i = start_pos2; ring_info.ip_arr[i] != '\0'; i++)
        {
            ring_info.ip_arr[start_pos1] = ring_info.ip_arr[i];
            start_pos1 += 1;
        }

        for (i = start_pos1; ring_info.ip_arr[i] != '\0'; i++)
            ring_info.ip_arr[i] = '\0';
    }
    //now realocate arrays.
    ring_info.process_counter -= 1;
    ring_info.id_arr = (unsigned*) realloc(ring_info.id_arr, ring_info.process_counter * sizeof (unsigned));

    if (ring_info.id_arr == NULL)
    {
        print_allocate_failed();
        return false;
    }

    ring_info.ip_arr = (char*) realloc(ring_info.ip_arr, ring_info.process_counter * sizeof (char) * MESSAGE_MAX_LENGTH);

    if (ring_info.ip_arr == NULL)
    {
        print_allocate_failed();
        return false;
    }

    return true;
}

void run()
{

}
