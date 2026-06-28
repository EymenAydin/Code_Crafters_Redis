#include "../include/timing.h"

int64_t get_millis(){
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((int64_t)now.tv_sec) * 1000 + ((int64_t)now.tv_nsec) / 1000000;
}
