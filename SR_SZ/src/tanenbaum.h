#ifndef TANENBAUM_H_
#define TANENBAUM_H_

//If there is no ring we want to join, we have to wait for other process to create it.
void wait_for_others(const unsigned time);

//Join another ring which exist.
void join_the_ring(const char* ip, const unsigned port);
#endif
