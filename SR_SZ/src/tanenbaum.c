#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int get_idx_from_id_arr(unsigned id)
{
    //we return idx if we find id in array, otherwise we return -1.
    int i;
    for (i = 0; i < ring_info.process_counter; i++)
    {
        if (id == ring_info.id_arr[i])
            return i;
    }

    return -1;
}

void handle_message(struct Message *msg)
{
    find_ip(msg -> sender_id, ring_info.tmp_ip);
    print_received_message_from(msg -> sender_id, ring_info.tmp_ip, msg -> type);

    switch (msg -> type)
    {
    case EmptyMessage:
        return;

    case ConnectionRequest:
        handle_ConnectionRequest(msg);
        msg -> type = EmptyMessage;
        return;

        //Shouldn't be here, put it here just in case. It's handled in other place.
    case ConnectionAccepted:
        print_message_should_not_be_handled();
        msg -> type = EmptyMessage;
        return;

    case Election:
        msg -> type = EmptyMessage;
        return;

    case CheckConnection:
        handle_CheckConnection(msg);
        msg -> type = EmptyMessage;
        return;

    case AddProcess:
        handle_AddProcess(msg);
        msg -> type = EmptyMessage;
        return;

    case RemoveProcess:
        handle_RemoveProcess(msg);
        msg -> type = EmptyMessage;
        return;

    case RequestRingInfo:
        handle_RequestRingInfo(msg);
        msg -> type = EmptyMessage;
        return;

        //It's also handled in other place.
    case SomeRingInfo:
        print_message_should_not_be_handled();
        return;
    }
}

bool handle_AddProcess(struct Message *msg)
{
    if (!send_message_to_next_process(msg))
        return false;

    if (!add_new_process_to_ring(msg -> ip))
    {
        msg -> type = RemoveProcess;
        msg -> original_sender_id = ring_info.process_id;
        convert_int_to_string(msg -> ip, ring_info.id_counter);
        send_message_to_next_process(msg);
        wait_for_specific_message(10, RemoveProcess, msg);
        return false;
    }

    return true;
}

bool handle_CheckConnection(struct Message *msg)
{
    send_message_to_next_process(msg);
    return true;
}

bool handle_ConnectionRequest(struct Message *msg)
{
    if (add_new_process_to_ring(msg -> ip))
    {
        msg -> type = AddProcess;
        msg -> original_sender_id = ring_info.process_id;
        send_message_to_next_process(msg);
    }

    if (wait_for_specific_message(10, AddProcess, msg))
    {
        msg -> type = ConnectionAccepted;
        sendMessage(&ring_info.socket, msg, ring_info.process_id, msg->ip, &ring_info.port);
        return true;
    }

    return false;
}

bool handle_RemoveProcess(struct Message *msg)
{
    remove_process_from_ring(atoi(msg -> ip));

    if (!send_message_to_next_process(msg))
        return false;

    return true;
}

bool handle_RequestRingInfo(struct Message *msg)
{
    //it could be removed meanwhile.
    if (!get_idx_from_id_arr(msg -> sender_id))
        return false;

    int tmp = get_idx_from_id_arr(atoi(msg -> ip));
    if (tmp == -1)
        return false;

    find_ip((unsigned)tmp, msg -> ip);
    find_ip(msg -> sender_id, ring_info.tmp_ip);
    sendMessage(&ring_info.socket, msg, ring_info.process_id, ring_info.tmp_ip, &ring_info.port);
    return true;

}

bool prepare_process(bool is_start_node, const unsigned time_cc, const unsigned time_cl, const unsigned *port, const char *ip)
{
    //At start we don't have any process in the ring.
    ring_info.process_counter = 0;
    ring_info.id_counter = 0;
    ring_info.port = *port;
    ring_info.checkConnection_time = time_cc;
    ring_info.checkLeader_time = time_cl;

    //1. Create a message which will handle the first message.
    struct Message msg;
    msg.type = EmptyMessage;

    /* 2a. If it's start mode and we wait for another process to connect and create a ring,
     * wait for other process to connect. This process became a leader. It's ID is 0.
     */
    if (is_start_node == true)
    {
        //Create a socket
        if (createAndBindSocket(&ring_info.socket, port) == NotWorking)
            return false;

        ring_info.is_leader = true;
        ring_info.leader_id = 0;
        ring_info.process_id = 0;
        add_new_process_to_ring("127.0.0.1");

        while(1)
        {
            checkMessageBox(&ring_info.socket, &msg);

            if(msg.type == ConnectionRequest)
            {
                if(add_new_process_to_ring(msg.ip))
                {                    
                    msg.original_sender_id = ring_info.id_arr[ring_info.process_counter];
                    print_received_message_from(msg.original_sender_id, msg.ip, msg.type);
                    msg.type = ConnectionAccepted;
                    convert_int_to_string(msg.ip, ring_info.process_counter);
                    //we use ip field to send how many processes are in the ring.
                    find_ip(ring_info.id_arr[ring_info.process_counter], ring_info.tmp_ip);
                    sendMessage(&ring_info.socket, &msg, ring_info.process_id, ring_info.tmp_ip, port);
                    print_sending_message_to(ring_info.id_arr[ring_info.process_counter], ring_info.tmp_ip, msg.type);
                    msg.type = EmptyMessage;
                }

                break;
            }
        }
    }

    /* 2b. If it's not a start mode, we send request for conenction to other process.
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
                print_received_message_from(msg.sender_id, "Not known yet", msg.type);
                ring_info.process_id = msg.original_sender_id;
                //Because ip field is unsued in this case, it's used to send other info.
                ring_info.process_counter = atoi(msg.ip);
                call_for_info_about_other_processes(ip);
                msg.type = EmptyMessage;
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
        //if there is not such id in array, just return false. Maybe AddProcess didn't reach this process.
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
    print_process_works(ring_info.port, ring_info.process_id);
}

bool send_message_to_next_process(struct Message *msg)
{
    if (msg -> original_sender_id == ring_info.process_id)
        return false;

    unsigned tmp_id;

    //Jump to the beggining  of array if this process is the last process in array.
    if (get_idx_from_id_arr(ring_info.process_id) == (ring_info.process_counter - 1))
    {
        tmp_id = check_if_to_avoid_process(ring_info.id_arr, ring_info.process_id + 1, msg -> ip);
        find_ip(tmp_id, ring_info.tmp_ip);
        print_sending_message_to(tmp_id, ring_info.tmp_ip, msg -> type);
    }

    //Otherwise just send it to the next process in array
    else
    {
        tmp_id = check_if_to_avoid_process(ring_info.id_arr, (get_idx_from_id_arr(ring_info.process_id) + 1), msg -> ip);
        find_ip(tmp_id, ring_info.tmp_ip);
        print_sending_message_to(tmp_id, ring_info.tmp_ip, msg -> type);
    }

    sendMessage(&ring_info.socket, msg, ring_info.process_id, ring_info.tmp_ip, &ring_info.port);
    return true;
}

bool wait_for_specific_message(unsigned sec, enum MessageType msgType, struct Message *msg)
{
    int i = 0;
    for (i = 0; i < sec; i++)
    {
        while(msg -> type != EmptyMessage)
        {
            checkMessageBox(&ring_info.socket, msg);

            if (msg -> type == msgType && msg -> original_sender_id == ring_info.process_id)
                return true;

            handle_message(msg);
        }

        sleep(1);
    }

    return false;
}
