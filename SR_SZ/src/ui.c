#include "ui.h"
#include "stdio.h"

void print_help()
{
    printf("Example of Tanenbaum algorithm. Usage:\n"
           "-ip <ip>        -- provide an ip address to which we want to connect\n"
           "-port <port>    -- provide a port to which we want to connect. Value has to be bigger than 0\n"
           "-start          -- this is first process in topology, let's start and wait for others\n"
           "-time <sec>     -- set the time of frequency of checking connection\n");
}
