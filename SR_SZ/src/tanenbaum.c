#include "tanenbaum.h"
#include <stdio.h>

void wait_for_others(const unsigned time)
{
    printf("URUCHOMIONO_TRYB_SINGLA:%u", time);
}
void join_the_ring(const char* ip, const unsigned port)
{
    printf("URUCHOMIONO_TRYB_MULTI:%s, %u", ip, port);
}
