#include <stdlib.h>
#include <string.h>

#include "tanenbaum.h"

bool add_new_process_to_ring(const char* ip)
{
    //inc the count of processes in the ring.
    ring_info.process_counter += 1;
    //raise the memory reserved by ptr for ids array and add new ID and ip to array.
    ring_info.id_arr = (unsigned*) realloc(ring_info.id_arr, ring_info.process_counter * sizeof (unsigned));
    ring_info.ip_arr = (char*) realloc(ring_info.ip_arr, ring_info.process_counter * sizeof (char) * MESSAGE_MAX_LENGTH);

    if (ring_info.id_arr == NULL || ring_info.ip_arr == NULL)
    {
        ring_info.process_counter -= 1;
        print_allocate_failed();
        return false;
    }

    ring_info.id_arr[ring_info.process_counter - 1] = ring_info.id_counter;
    strcat(ring_info.ip_arr, ip);
    strcat(ring_info.ip_arr, ";");
    ring_info.id_counter += 1;
    print_added_new_process(ip);
    return true;
}

void call_for_info_about_other_processes(const char *ip)
{
    struct Message msg;
    msg.type = EmptyMessage;
    unsigned i;
    bool break_loop;

    for (i = 0; i < ring_info.process_counter; i++)
    {
        msg.type = RequestRingInfo;
        convert_int_to_string(msg.ip, i);
        sendMessage(&ring_info.socket, &msg, ring_info.process_id, ip, &ring_info.port);
        msg.type = EmptyMessage;
        break_loop = false;

        while(!break_loop)
        {
            checkMessageBox(&ring_info.socket, &msg);
            if (msg.type == SomeRingInfo)
                break_loop = true;

            handle_message(&msg);
        }
    }
}

void find_ip(unsigned id, char* ip)
{
    /* Because array of ids can look like [1,3,5] and array of ips is like <ip>;<ip>;<ip>;
     * we have to iterate through id array and when we find right id, then ip is in the place,
     * which which correspond to index of that id in array.
     */
    unsigned i, pos, j = 0, counter = 0; //counter counts how many ; we got.

    for (i = 0; i < ring_info.process_counter; i++)
    {
        if (ring_info.id_arr[i] == id)
        {
            pos = i;
            break;
        }
    }

    for (i = 0; ring_info.ip_arr[i] != '\0'; i++)
    {
        if (pos == counter)
        {
            ip[j] = ring_info.ip_arr[i];
            j += 1;
        }

       if (ring_info.ip_arr[i] == ';')
            counter += 1;

       if (counter > id)
           break;
    }

    //In the end remove ';' from the end of string.
    ip[j - 1] = '\0';
}

void handle_message(struct Message *msg)
{
    switch (msg -> type)
    {
    case EmptyMessage:
        return;

    case ConnectionRequest:
        return;

        //Shouldn't be here, put it here just in case. It's handled in other place.
    case ConnectionAccepted:
        return;

    case Election:
        return;

    case CheckConnection:
        return;

    case ConnectionsConfirm:
        return;

    case AddProcess:
        return;

    case RemoveProcess:
        return;

    case RequestRingInfo:
        return;

        //It's also handled in other place.
    case SomeRingInfo:
        return;
    }
}

bool prepare_process(bool is_start_node, const unsigned time, const char *ip, const unsigned *port)
{
    //At start we don't have any process in the ring.
    ring_info.process_counter = 0;
    ring_info.id_counter = 0;
    ring_info.port = *port;

    //1.Create a socket
    if (createAndBindSocket(&ring_info.socket, port) == NotWorking)
        return false;

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
            checkMessageBox(&ring_info.socket, &msg);

            if(msg.type == ConnectionRequest)
            {
                if(add_new_process_to_ring(msg.ip))
                {
                    msg.type = ConnectionAccepted;
                    msg.original_sender_id = ring_info.id_arr[ring_info.process_counter];
                    convert_int_to_string(msg.ip, ring_info.process_counter);
                    //we use ip field to send how many processes are in the ring.
                    find_ip(ring_info.id_arr[ring_info.process_counter], ring_info.tmp_ip);
                    sendMessage(&ring_info.socket, &msg, ring_info.process_id, ring_info.tmp_ip, port);
                    msg.type = EmptyMessage;
                }

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
        sendConnectionRequest(&ring_info.socket, ip, port);

        while(1)
        {
            checkMessageBox(&ring_info.socket, &msg);

            if(msg.type == ConnectionAccepted)
            {
                ring_info.process_id = msg.original_sender_id;
                //Because ip field is unsued in this case, it's used to send other info.
                ring_info.process_counter = atoi(msg.ip);
                call_for_info_about_other_processes(ip);
                break;
            }
        }
    }

    return true;
}

void remove_all_processes_from_ring()
{
    int i;
    for (i = 0; i < ring_info.process_counter; i++)
        remove_process_from_ring(ring_info.id_arr[i]);
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
    unsigned i, ip_pos;
    bool found_pos = false;
    for (i = 0; i < ring_info.process_counter; i++)
    {
        if (ring_info.id_arr[i] == id)
        {
            found_pos = true;
            ip_pos = i;
        }

        //if id is not on last pos.
        if (found_pos == true && i < ring_info.process_counter - 1)
        {
            ring_info.id_arr[i] = ring_info.id_arr[i + 1];
            if (i == ring_info.process_counter - 2)
                break;
        }
    }

    if (!found_pos)
    {
        //if there is not such id in array, terminate process, something is wrong.
        free(ring_info.id_arr);
        free(ring_info.ip_arr);
        return false;
    }

    //find a place where ip to remove starts, when we find it, start rewriting from that pos.
    unsigned counter = 0, start_pos1 = 0, start_pos2 = 0;
    char ip_to_remove[16];
    found_pos = false;

    for (i = 0; ring_info.ip_arr[i] != '\0'; i++)
    {
        if (ip_pos == counter && !found_pos)
        {
            start_pos1 = i;
            found_pos = true;
        }

        if (ring_info.ip_arr[i] == ';')
            counter += 1;

        if (counter > ip_pos)
        {
            start_pos2 = i;
            break;
        }

        if (found_pos)
            ip_to_remove[i - start_pos1] = ring_info.ip_arr[i];
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

    print_remove_process(&id, ip_to_remove);
    return true;
}

void run()
{
    print_process_works(ring_info.port);
}
