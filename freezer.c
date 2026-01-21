#include <signal.h>
#include <unistd.h>
#include "common.h"

void freeze_process(pid_t pid) {
    kill(pid, SIGSTOP);
    usleep(50000);  // allow state to settle
}
