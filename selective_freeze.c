#include <stdio.h>
#include <string.h>
#include "common.h"

int is_safe_to_freeze(pid_t pid) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/io", pid);

    FILE *f = fopen(path, "r");
    if (!f) return 1;

    char line[256];
    unsigned long write_bytes = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "write_bytes:", 12) == 0) {
            sscanf(line + 12, "%lu", &write_bytes);
            break;
        }
    }

    fclose(f);
    return write_bytes == 0;
}
