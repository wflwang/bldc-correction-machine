/*
 * syscalls.c
 * System call implementations for embedded systems
 */

#include <stdint.h>
#include <stddef.h>
#include <sys/time.h>

/*
 * Minimal system call implementations to satisfy linker
 */

/* Get current time */
int _gettimeofday(struct timeval *tv, void *tz) {
    (void)tv;
    (void)tz;
    return 0;
}

/* Breakpoint */
void __breakpoint(void) {
    while (1);
}

/* Exit */
void _exit(int status) {
    (void)status;
    while (1);
}

/* Kill */
int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    return -1;
}

/* Getpid */
int _getpid(void) {
    return 1;
}

/* Wait */
int _wait(int *status) {
    (void)status;
    return -1;
}

/* Raise */
int raise(int sig) {
    (void)sig;
    return -1;
}
