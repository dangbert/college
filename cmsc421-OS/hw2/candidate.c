/**
 * @Author Dan Engbert (end1@umbc.edu)
 * This file contains a program that counts the number of votes a candidate receives.
 * A vote is received when a SIGUSR1 signal is received.
 * When a SIGUSR2 signal is received, the vote count is outputted to stdout as bytes.
 *
 * Help received:
 * https://www.csee.umbc.edu/~jtang/cs421.f18/lectures/L06IPC.pdf#page=19
 * https://bash.cyberciti.biz/guide/Sending_signal_to_Processes
 * https://ubuntuforums.org/showthread.php?t=1430052
 * man page: write function
 */

#define _POSIX_SOURCE
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

unsigned int count = 0; // global (4 bytes in size)

/**
 * increment count when a SIGUSR1 signal is received
 */
static void fraud(int signum) {
    /*
    printf("starting fraud...\n");
    char num[4];
    int res = read(fileno(stdin), &num, 4);
    printf("res = %d\n", res);

    if (read(fileno(stdin), &num, 4) != 4)
        printf("failed to read furing fraud\n");
    printf("  desired fraud value: %x, %x, %x, %x (hex) votes\n", num[0], num[1], num[2], num[3]);
    // TODO update count
    */
}

/**
 * increment count when a SIGUSR1 signal is received
 */
static void incrementCount(int signum) {
    count++;
}

/**
 * print count as 32 bits (instead of ASCII) to stderr
 * when a SIGUSR2 signal is received
 */
static void printCount(int signum) {
    fflush(stdout); // flush buffer (for printf)
    int written = write(fileno(stderr), &count, sizeof(count));
    if (written != 4) {
        printf("Error writing to stderr, write() returned: %d (4 expected).\n", written);
    }
}

int main(int argc, char *argv[]) {
    // validate and parse args
    if (argc == 1) {
        printf("No candidate name given.\n");
        exit(1);
    }
    else if (argc > 2) {
        printf("Only one arg is needed.\n");
        exit(1);
    }
    printf("I am %s at PID %d.\n", argv[1], getpid());

    // set incrementCount() to be called when a SIGUSR1 is received
    sigset_t mask;
    sigemptyset(&mask);
    struct sigaction sa1 = {
        .sa_handler = incrementCount,
        .sa_mask = mask,
        .sa_flags = 0
    };
    sigaction(SIGUSR1, &sa1, NULL);
    struct sigaction sa2 = {
        .sa_handler = printCount,
        .sa_mask = mask,
        .sa_flags = 0
    };
    sigaction(SIGUSR2, &sa2, NULL);
    struct sigaction sa3 = {
        .sa_handler = fraud,
        .sa_mask = mask,
        .sa_flags = 0
    };
    sigaction(SIGALRM, &sa3, NULL);
    // TODO: do something with ctrl-d

    // send signal with: kill -s SIGUSR1 `pidof ./candidate`
    // infinite loop (waiting for signals)
    while (1) {
        errno = 0;
        // this is a system call, if it gets interrupted by a signal errno will be set to 4
        // we want to terminate the program when stdin is closed (not when a signal is recieved)
        // read from stdin
        char buf[100];
        if (fgets(buf, sizeof(buf), stdin) == NULL && errno !=4) {
            printf("PID %d terminating.\n", getpid());
            return 0;
        }
    }

    return 0;
}
