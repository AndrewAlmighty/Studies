#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "ui.h"
#include "tanenbaum.h"

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

int main(int argc, char** argv)
{
    if (argc < 2)
        print_help();

    else
    {
        int idx;
        bool got_ip = false;
        bool got_port = false;
        bool got_time = false;
        bool is_start_node = false;

        char *ip = NULL;
        unsigned port = 0;
        unsigned time = 10;

        for (idx = 1; idx < argc; idx++)
        {
            if (strcmp(argv[idx], "-ip") == 0 && got_ip)
            {
                print_help();
                break;
            }

            else if (strcmp(argv[idx], "-ip") == 0 && !got_ip && (idx + 1) < argc)
            {
                got_ip = check_ip(argv[idx + 1], &ip);

                if(!got_ip)
                    break;

                continue;
            }

            if (strcmp(argv[idx], "-port") == 0 && got_port)
                break;

            else if (strcmp(argv[idx], "-port") == 0 && !got_port && (idx + 1) < argc)
            {
                got_port = check_port(argv[idx + 1], &port);

                if(!got_port)
                    break;

                continue;
            }

            if (strcmp(argv[idx], "-time") == 0 && got_time)
                break;

            else if (strcmp(argv[idx], "-time") == 0 && !got_time && (idx + 1) < argc)
            {
                got_time = check_time(argv[idx + 1], &time);

                if(!got_time)
                    break;

                continue;
            }

            if (strcmp(argv[idx], "-start") == 0 && is_start_node)
                break;

            else if (strcmp(argv[idx], "-start") == 0 && !is_start_node)
                is_start_node = true;
        }

        if (is_start_node)
            wait_for_others(time);

        else if (got_ip && got_port)
            join_the_ring(ip, port);

        else
            print_help();
    }

    return 0;
}
