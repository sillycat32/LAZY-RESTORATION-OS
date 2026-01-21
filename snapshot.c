#define _POSIX_C_SOURCE 200809L
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <stdio.h>
#include "common.h"

void snapshot_process(pid_t pid) {
    char dir[128], src[64];

    mkdir("snapshots", 0755);
    snprintf(dir, sizeof(dir), "snapshots/%d", pid);
    mkdir(dir, 0755);

    snprintf(src, sizeof(src), "/proc/%d/maps", pid);

    int in = open(src, O_RDONLY);
    int out = openat(open(dir, O_DIRECTORY),
                     "maps", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (in < 0 || out < 0) return;

    off_t off = 0;
    sendfile(out, in, &off, 1 << 20);

    close(in);
    close(out);
}
