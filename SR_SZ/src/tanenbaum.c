#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
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
    print_added_new_process(ring_info.id_arr[ring_info.process_counter - 1], ip);
    strcat(ring_info.ip_arr, ip);
    strcat(ring_info.ip_arr, ";");
    ring_info.id_counter += 1;
    return true;
}

void call_for_info_about_other_processes(const char *ip, const unsigned node_id)
{
    struct Message msg;
    msg.type = EmptyMessage;
    unsigned i, nodes_in_ring = ring_info.process_counter;
    ring_info.process_counter = 0;
    bool break_loop;

    for (i = 0; i < nodes_in_ring; i++)
    {
        msg.type = RequestRingInfo;
        msg.auxiliary_int = i;
        sendMessage(&ring_info.socket, &msg, ring_info.process_id, ip, &ring_info.port);
        msg.type = EmptyMessage;
        break_loop = false;
        while(!break_loop)
        {
            checkMessageBox(&ring_info.socket, &msg);
            if (msg.type == SomeRingInfo)
                break_loop = true;

            handle_message(&msg, false);
        }
    }
}

bool are_enough_process_to_continue()
{
    if (ring_info.process_counter > 1)
        return true;

    else return false;
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

void handle_message(struct Message *msg, bool is_ready)
{
    if ((msg -> type != EmptyMessage && msg -> type != SomeRingInfo) && is_ready)
    {
        find_ip(msg -> sender_id, ring_info.tmp_ip);
        print_received_message_from(msg -> sender_id, ring_info.tmp_ip, msg -> type);
    }

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

    case SomeRingInfo:
        handle_SomeRingInfo(msg);
        msg -> type = EmptyMessage;
        return;
    }
}

bool handle_AddProcess(struct Message *msg)
{
    if (!send_message_to_next_process(msg))
    {
        msg -> auxiliary_int = -1;
        return false;
    }

    if (!add_new_process_to_ring(msg -> text))
    {
        msg -> type = RemoveProcess;
        msg -> original_sender_id = ring_info.process_id;
        msg -> auxiliary_int = ring_info.id_counter;
        send_message_to_next_process(msg);
        wait_for_specific_message(10, RemoveProcess, msg);
        return false;
    }

    return true;
}

bool handle_CheckConnection(struct Message *msg)
{
    ring_info.leader_id = msg -> original_sender_id;
    send_message_to_next_process(msg);
    return true;
}

bool handle_ConnectionRequest(struct Message *msg)
{
    if (add_new_process_to_ring(msg -> text))
    {
        msg -> type = AddProcess;
        msg -> original_sender_id = ring_info.process_id;
        msg -> sender_id = ring_info.process_id;
        msg -> auxiliary_int = ring_info.id_arr[ring_info.process_counter - 1];
        //we don't want to send this message to new process, so temporary reduce process_counter by one.
        send_message_to_next_process(msg);
        msg -> auxiliary_int = - 1;
    }

    if (wait_for_specific_message(10, AddProcess, msg))
    {
        msg -> type = ConnectionAccepted;
        msg -> original_sender_id = ring_info.id_arr[ring_info.process_counter - 1];
        find_ip(ring_info.id_arr[ring_info.process_counter - 1], ring_info.tmp_ip);
        msg -> auxiliary_int = ring_info.process_counter;
        sendMessage(&ring_info.socket, msg, ring_info.process_id, ring_info.tmp_ip, &ring_info.port);
        print_sending_message_to(msg -> original_sender_id, ring_info.tmp_ip, msg -> type);
        return true;
    }

    return false;
}

bool handle_RemoveProcess(struct Message *msg)
{
    if (!send_message_to_next_process(msg))
    {
        msg -> auxiliary_int = -1;
        return false;
    }

    remove_process_from_ring(msg -> auxiliary_int);
    return true;
}

bool handle_RequestRingInfo(struct Message *msg)
{
    //it could be removed meanwhile.
    if (!get_idx_from_id_arr(msg -> sender_id))
        return false;

    int tmp = get_idx_from_id_arr(msg -> auxiliary_int);
    if (tmp == -1)
        return false;

    find_ip((unsigned)tmp, msg -> text);
    msg -> original_sender_id = (unsigned)tmp;  //we send id of process with that field.
    find_ip(msg -> sender_id, ring_info.tmp_ip);
    msg -> type = SomeRingInfo;
    print_sending_message_to(msg -> sender_id, ring_info.tmp_ip, msg -> type);
    sendMessage(&ring_info.socket, msg, ring_info.process_id, ring_info.tmp_ip, &ring_info.port);    
    msg -> auxiliary_int = -1;
    return true;
}

bool handle_SomeRingInfo(struct Message *msg)
{
    add_new_process_to_ring(msg -> text);
    ring_info.id_arr[ring_info.process_counter - 1] = msg -> original_sender_id;
    return true;
}

bool prepare_process(bool is_start_node, const unsigned time_cc, const unsigned time_cl, const unsigned *port, const char *my_ip, const char *ip)
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
    msg.auxiliary_int = -1;

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
        add_new_process_to_ring(my_ip);

        while(1)
        {
            checkMessageBox(&ring_info.socket, &msg);

            if(msg.type == ConnectionRequest)
            {
                if(add_new_process_to_ring(msg.text))
                {
                    msg.original_sender_id = ring_info.id_arr[ring_info.process_counter - 1];
                    print_received_message_from(msg.original_sender_id, msg.text, msg.type);
                    msg.type = ConnectionAccepted;
                    msg.auxiliary_int = ring_info.process_counter;
                    //we use ip field to send how many processes are in the ring.
                    find_ip(ring_info.id_arr[ring_info.process_counter - 1], ring_info.tmp_ip);
                    sendMessage(&ring_info.socket, &msg, ring_info.process_id, ring_info.tmp_ip, port);
                    print_sending_message_to(ring_info.id_arr[ring_info.process_counter - 1], ring_info.tmp_ip, msg.type);
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
                ring_info.process_counter = msg.auxiliary_int;
                call_for_info_about_other_processes(ip, msg.sender_id);
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
    char ip_to_remove[16] = "";
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
            start_pos2 = i + 1;
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
    struct Message msg;
    msg.type = EmptyMessage;

    bool checking_connection = false;
    // bool checking_leader = false;
    unsigned stopwatch_cc = 0;
    //  unsigned stopwatch_cl = 0;
    bool mailbox_checked, keep_running = true;

    while (keep_running)
    {
        mailbox_checked = false;
        if (ring_info.is_leader)
        {
            //time to check connection. We send this message to next process.
            if (!checking_connection && stopwatch_cc >= ring_info.checkConnection_time)
            {
                checking_connection = true;
                msg.type = CheckConnection;
                //We have to reset sender id because it will cause problem in next method.
                msg.original_sender_id = msg.sender_id = ring_info.process_id;
                msg.auxiliary_int = -1;
                send_message_to_next_process(&msg);
                msg.type = EmptyMessage;
            }

            else if (!checking_connection && stopwatch_cc < ring_info.checkConnection_time)
                stopwatch_cc += 1;

            if (checking_connection)
            {
                unsigned i = 0;
                int idx_of_suspect_process_in_arr = -1;

                while(1)
                {
                    /* We send message and it should come back here in x sec. If it doesn't come back,
                     * it means that some process is not working. Then we send message again, but we
                     * avoid one process. If then message come back to us, that means we found a process
                     * which is not working, so we remove it from the ring. If it still doesn't come back,
                     * then we iterate through all ring and if we don't find it, we terminate app because
                     * something is bad.
                     */                    

                    if (wait_for_specific_message(10, CheckConnection, &msg))
                    {
                        stopwatch_cc = 0;
                        checking_connection = false;

                        if (idx_of_suspect_process_in_arr >= 0)
                        {
                            if (!remove_process_from_ring(msg.auxiliary_int))
                            {
                                print_terminate("Error while removing process ...");
                                remove_all_processes_from_ring();
                                return;
                            }

                            struct Message msg;
                            msg.type = RemoveProcess;                            
                            msg.original_sender_id = msg.sender_id = ring_info.process_id;
                            send_message_to_next_process(&msg);
                        }

                        break;
                    }

                    else
                    {
                        if (ring_info.process_counter <= 2)
                        {
                            print_terminate("Only one process left in ring");
                            remove_all_processes_from_ring();
                            return;
                        }

                        print_some_process_doesnt_work();
                        //we check processes from beggining of the array, so if we have our process, jump it.
                        idx_of_suspect_process_in_arr = get_idx_from_id_arr(ring_info.process_id);
                        if (ring_info.process_id == ring_info.id_arr[ring_info.process_counter - 1 - i])
                            i += 1;

                        msg.type = CheckConnection;
                        msg.auxiliary_int = ring_info.id_arr[ring_info.process_counter - 1 - i];
                        msg.original_sender_id = msg.sender_id = ring_info.process_id;
                        send_message_to_next_process(&msg);
                        msg.type = EmptyMessage;
                    }

                    i += 1;

                    if (i >= ring_info.process_counter)
                    {
                        print_terminate("Could find not working process");
                        return;
                    }
                }
            }
        }

        while(msg.type != EmptyMessage || !mailbox_checked)
        {
            handle_message(&msg, true);
            if (msg.type == ConnectionRequest)
                stopwatch_cc = 0;

            checkMessageBox(&ring_info.socket, &msg);
            mailbox_checked = true;
        }

        sleep(1);
        keep_running = are_enough_process_to_continue();
    }

    print_terminate("Not enought process to continue");
}

bool send_message_to_next_process(struct Message *msg)
{
    if (msg -> original_sender_id == ring_info.process_id && msg -> sender_id != ring_info.process_id)
        return false;

    unsigned tmp_id, idx_to_check;

    //Jump to the beggining  of array if this process is the last process in array.
    if (ring_info.id_arr[get_idx_from_id_arr(ring_info.process_id)] == ring_info.id_arr[ring_info.process_counter - 1])
        idx_to_check = 0;

    //Otherwise just send it to the next process in array
    else
        idx_to_check = get_idx_from_id_arr(ring_info.process_id) + 1;

    tmp_id = check_if_to_avoid_process(ring_info.id_arr, ring_info.process_counter, idx_to_check, msg ->auxiliary_int);
    tmp_id = check_if_to_avoid_process(ring_info.id_arr, ring_info.process_counter, tmp_id, ring_info.process_id);
    find_ip(tmp_id, ring_info.tmp_ip);
    print_sending_message_to(tmp_id, ring_info.tmp_ip, msg -> type);
    sendMessage(&ring_info.socket, msg, ring_info.process_id, ring_info.tmp_ip, &ring_info.port);
    msg -> type = EmptyMessage;

    return true;
}

bool wait_for_specific_message(unsigned sec, enum MessageType msgType, struct Message *msg)
{
    int i = 0;
    bool checked_msg_box;
    for (i = 0; i < sec; i++)
    {
        checked_msg_box = false;
        while(msg -> type != EmptyMessage || !checked_msg_box)
        {
            handle_message(msg, true);
            checkMessageBox(&ring_info.socket, msg);
            if (msg -> type == msgType && msg -> original_sender_id == ring_info.process_id)
            {
                handle_message(msg, true);
                return true;
            }

        checked_msg_box = true;
        }

        sleep(1);
    }

    return false;
}
