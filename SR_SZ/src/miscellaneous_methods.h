#ifndef MISCELLANEOUS_METHODS_H_
#define MISCELLANEOUS_METHODS_H_

#include <stdbool.h>

bool check_ip(const char* ip_from_arg, char** ip);
bool check_port(const char *arg_port, unsigned *port);
bool check_time(const char *arg_time, unsigned *time);
void print_allocate_failed();
void print_help();
void print_process_works(unsigned port);
void print_remove_from_arr_failed();

#endif
