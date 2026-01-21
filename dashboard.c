#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "common.h"

static pid_t target_pid;
static char exe_path[256];

static double lazy_t = -1, eager_t = -1, cold_t = -1;

/* ---------- Helpers ---------- */

int get_exe(pid_t pid, char *buf, size_t sz) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    ssize_t len = readlink(path, buf, sz - 1);
    if (len < 0) return -1;
    buf[len] = '\0';
    return 0;
}

void wait_until_alive(pid_t pid) {
    for (int i = 0; i < 50; i++) {
        if (kill(pid, 0) == 0) return;
        usleep(10000);
    }
}

void draw_bar(int row, const char *label, double val, double max) {
    int width = 30;
    int len = (int)((val / max) * width);

    mvprintw(row, 2, "%s |", label);
    for (int i = 0; i < len; i++) addch('#');
    for (int i = len; i < width; i++) addch(' ');
    printw("| %.2f ms", val);
}

/* ---------- UI ---------- */

void draw_ui() {
    clear();

    mvprintw(1, 2, "Zero-Latency Application Switcher");
    mvprintw(2, 2, "Executable: %s", exe_path);
    mvprintw(3, 2, "Target PID: %d", target_pid);

    mvprintw(5, 2, "[S] Snapshot & Hibernate");
    mvprintw(6, 2, "[L] Lazy Restore");
    mvprintw(7, 2, "[E] Eager Restore");
    mvprintw(8, 2, "[C] Cold Start");
    mvprintw(9, 2, "[Q] Quit");

    mvprintw(11, 2, "Latency (ms):");
    mvprintw(12, 4, "Lazy  : %.2f",  lazy_t  > 0 ? lazy_t  : 0.0);
    mvprintw(13, 4, "Eager : %.2f", eager_t > 0 ? eager_t : 0.0);
    mvprintw(14, 4, "Cold  : %.2f",  cold_t  > 0 ? cold_t  : 0.0);

    double max = 0;
    if (lazy_t  > max) max = lazy_t;
    if (eager_t > max) max = eager_t;
    if (cold_t  > max) max = cold_t;

    if (max > 0) {
        draw_bar(16, "Lazy ",  lazy_t,  max);
        draw_bar(17, "Eager", eager_t, max);
        draw_bar(18, "Cold ",  cold_t,  max);
    }

    refresh();
}

/* ---------- Main ---------- */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    target_pid = atoi(argv[1]);

    if (kill(target_pid, 0) != 0 ||
        get_exe(target_pid, exe_path, sizeof(exe_path)) < 0) {
        printf("Invalid or dead PID\n");
        return 1;
    }

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    int ch;
    while (1) {
        draw_ui();
        ch = getch();

        if (ch == 'q' || ch == 'Q') break;

        if (ch == 's' || ch == 'S') {
            freeze_process(target_pid);
            snapshot_process(target_pid);
            kill(target_pid, SIGKILL);
        }

        else if (ch == 'l' || ch == 'L') {
            double t1 = now_ms();
            pid_t p = restore_process(exe_path, 1);
            wait_until_alive(p);
            lazy_t = now_ms() - t1;
            target_pid = p;
        }

        else if (ch == 'e' || ch == 'E') {
            double t1 = now_ms();
            pid_t p = restore_process(exe_path, 0);
            wait_until_alive(p);
            eager_t = now_ms() - t1;
            target_pid = p;
        }

        else if (ch == 'c' || ch == 'C') {
            double t1 = now_ms();
            pid_t p = fork();
            if (p == 0) {
                execl(exe_path, exe_path, NULL);
                _exit(1);
            }
            wait_until_alive(p);
            cold_t = now_ms() - t1;
            target_pid = p;
        }
    }

    endwin();
    return 0;
}
