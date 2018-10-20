#include <stdbool.h>
#include <string.h>

#include "miscellaneous_methods.h"
#include "tanenbaum.h"

int main(int argc, char** argv)
{
    //check arguments. We need ip and port or single mode on. If something is wrong, print help and terminate.
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
        unsigned port = 9000;
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
            prepare_process(true, time, ip, port);

        else if (got_ip)
            prepare_process(true, time, ip, port);

        else
            print_help();
    }

    return 0;
}
