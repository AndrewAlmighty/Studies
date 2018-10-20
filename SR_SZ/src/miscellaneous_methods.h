#ifndef MISCELLANEOUS_METHODS_H_
#define MISCELLANEOUS_METHODS_H_

#include <stdbool.h>

bool check_ip(const char* ip_from_arg, char** ip);
bool check_port(const char *arg_port, unsigned *port);
bool check_time(const char *arg_time, unsigned *time);
void convert_int_to_string(char* str, int integer);
void print_added_new_process(const char *ip);
void print_allocate_failed();
void print_help();
void print_process_works(unsigned port);
void print_remove_process(const unsigned *id, const char *ip);

#endif
