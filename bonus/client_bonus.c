#include "minitalkbonus.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t ack_received = 0;

void handler(int sig) {
    if (sig == SIGUSR1) {
        ack_received = 1;
    } else if (sig == SIGUSR2) {
        write(1, "mesaj ulasti\n", 13);
    }
}

void send_bits(char byte, int server_pid) {
    int i = 7;
    while (i >= 0) {
        ack_received = 0;
        if (byte >> i & 1)
            kill(server_pid, SIGUSR1);
        else
            kill(server_pid, SIGUSR2);
        while (!ack_received) {
            pause(); // Wait for acknowledgment
        }
        i--;
    }
}

void send_bytes(char *message, int server_pid) {
    int i = -1;
    while (message[++i]) {
        send_bits(message[i], server_pid);
    }
    send_bits('\0', server_pid); // Send null terminator to indicate end of message
}

int main(int argc, char **argv) {
    if (argc != 3) {
        write(1, "Usage: ./client_bonus [server_pid] [message]\n", 45);
        return 1;
    }

    int server_pid = ft_atoi(argv[1]);
    char *message = argv[2];

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    send_bytes(message, server_pid);

    return 0;
}