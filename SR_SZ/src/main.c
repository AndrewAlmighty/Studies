#include <signal.h>
#include <stdbool.h>
#include <string.h>

#include "miscellaneous_methods.h"
#include "tanenbaum.h"

int main(int argc, char** argv)
{
    //Connect signals to proper method which will handle terminate the program.
    signal(SIGINT, terminate);
    signal(SIGTSTP, instant_shutdown);

    //check arguments. We need ip and port or single mode on. If something is wrong, print help and terminate.
    if (argc < 2)
        print_help();

    else
    {
        int idx;
        bool got_ip = false;
        bool got_my_ip = false;
        bool got_port = false;
        bool got_time_cc = false;
        bool got_time_cl = false;
        bool is_start_node = false;
        bool can_run = false;

        char *ip = NULL;
        char *my_ip = NULL;
        unsigned port = 9000;
        unsigned time_cc = 15;
        unsigned time_cl = 40;

        for (idx = 1; idx < argc; idx++)
        {
            if (strcmp(argv[idx], "-my_ip") == 0 && got_my_ip)
            {
                print_help();
                break;
            }

            else if (strcmp(argv[idx], "-my_ip") == 0 && !got_my_ip && (idx + 1) < argc)
            {
                got_my_ip = check_ip(argv[idx + 1], &my_ip);

                if(!got_my_ip)
                    break;

                continue;
            }

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

            if (strcmp(argv[idx], "-time_cc") == 0 && got_time_cc)
                break;

            else if (strcmp(argv[idx], "-time_cc") == 0 && !got_time_cc && (idx + 1) < argc)
            {
                got_time_cc = check_time(argv[idx + 1], &time_cc);

                if (!got_time_cc)
                    break;

                continue;
            }

            if (strcmp(argv[idx], "-time_cl") == 0 && got_time_cl)
                break;

            else if (strcmp(argv[idx], "-time_cl") == 0 && !got_time_cl && (idx + 1) < argc)
            {
                got_time_cl = check_time(argv[idx + 1], &time_cl);

                if(!got_time_cl)
                    break;

                continue;
            }

            if (strcmp(argv[idx], "-start") == 0 && is_start_node)
                break;

            else if (strcmp(argv[idx], "-start") == 0 && !is_start_node)
                is_start_node = true;
        }

        if (is_start_node && got_my_ip)
            can_run = prepare_process(true, time_cc, time_cl, &port, my_ip, ip);

        else if (!is_start_node && got_ip)
            can_run = prepare_process(false, time_cc, time_cl, &port, my_ip, ip);

        else
            print_help();

        if(can_run)
            run();
    }

    return 0;
}
