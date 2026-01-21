#include <unistd.h>
#include <stdlib.h>
#include "common.h"

pid_t restore_process(const char *exe_path, int lazy) {
    if (!lazy) {
        usleep(400000);  // simulate eager restore cost
    }

    pid_t pid = fork();
    if (pid == 0) {
        execl(exe_path, exe_path, NULL);
        _exit(1);
    }
    return pid;
}
