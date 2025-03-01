#include "minitalkbonus.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

static int bit = 128;
static int client_pid = 0;
static char c = 0;

void reset_data(void) {
    bit = 128;
    client_pid = 0;
    c = 0;
}

void handler(int sig, siginfo_t *info, void *ucontext) {
    (void)ucontext;
    if (client_pid != info->si_pid) {
        reset_data();
        client_pid = info->si_pid;
    }
    if (sig == SIGUSR1) {
        c += bit;
    }
    bit /= 2;
    if (bit == 0) {
        write(1, &c, 1);
        if (c == '\0') {
            kill(info->si_pid, SIGUSR2); // Send acknowledgment signal to client
        }
        bit = 128;
        c = 0;
    }
    kill(info->si_pid, SIGUSR1); // Send acknowledgment signal to client for each bit received
}

int main(void) {
    struct sigaction sa;

    printf("Server PID: %d\n", getpid()); // Print the PID

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    while (1) {
        pause();
    }
    return 0;
}