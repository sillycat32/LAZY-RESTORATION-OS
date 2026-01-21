#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include "common.h"

double now_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1e6;
}
