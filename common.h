#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>

/* Process control */
void freeze_process(pid_t pid);
int is_safe_to_freeze(pid_t pid);

/* Snapshot */
void snapshot_process(pid_t pid);

/* Restore */
pid_t restore_process(const char *exe_path, int lazy);

/* Timing */
double now_ms(void);

#endif
