#ifndef MISCELLANEOUS_METHODS_H_
#define MISCELLANEOUS_METHODS_H_

#include <stdbool.h>

int check_if_to_avoid_process(unsigned *arr, unsigned arr_size, unsigned idx, int id_to_avoid);
bool check_ip(const char* ip_from_arg, char** ip);
bool check_port(const char *arg_port, unsigned *port);
bool check_time(const char *arg_time, unsigned *time);
void convert_int_to_string(char* str, int integer);
void get_str_of_message_type(char *str, const int type);
void print_added_new_process(const unsigned id, const char *ip);
void print_allocate_failed();
void print_help();
void print_leader_does_not_react();
void print_leader_works_other_process_dont();
void print_new_leader(unsigned id);
void print_message_should_not_be_handled();
void print_process_works(unsigned port, unsigned id);
void print_received_message_from(const int id, const char *ip, const int type);
void print_remove_process(const unsigned *id, const char *ip);
void print_sending_message_to(const unsigned id, const char *ip, const int type);
void print_some_process_doesnt_work();
void print_terminate(const char* reason);
void print_timeout_reached();

#endif
