/**
 * @Author Dan Engbert (end1@umbc.edu)
 * This file contains a program that creates 4 child processes which execute the
 * candidate program (each with one of the args passed to this program).
 *
 * Help received:
 * man pages: pipe, fork, dup2, close, execlp, waitpid, kill, scanf
 * https://stackoverflow.com/questions/21558937/i-do-not-understand-how-execlp-works-in-linux#
 */

#define _POSIX_SOURCE
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
extern char **environ; // for execlp()


int main(int argc, char *argv[]) {
    // validate args
    if (argc != 5) {
        printf("Exactly 4 args expected.\n");
        exit(1);
    }

    // array of 8 file descriptor pairs (2 pairs per process)
    // each pair: element 0: read end, element 1= write-end
    int fds[4][2][2];
    for (int i=0; i<4; i++) {
        // create pipes
        if (pipe(fds[i][0]) == -1)
            printf("Error creating initial pipe.\n");
        if (pipe(fds[i][1]) == -1)
            printf("Error creating initial pipe.\n");
    }
    pid_t children[4];

    for (int i=0; i<4; i++) {
        // child i is associated with the 2 pipe pairs (P1 and P2) in fds[i]
        pid_t pid = fork();
        if (pid == 0) {
            // child process:
            int* P1 = fds[i][0]; // P1 is for the child to read (from parent)
            int* P2 = fds[i][1]; // P2 is for the child to write (to parent)
            // close stdin and duplicate the reading end of pipe P1
            if (dup2(P1[0], fileno(stdin)) == -1)
                printf("Error duplicating P1[0] (i=%d)\n", i);
            // close reading end of P1
            if (close(P1[0]) == -1)
                printf("Error closing P1[0] (i=%d)\n", i);

            // close stderr and duplicate the writing end of pipe P2
            if (dup2(P2[1], fileno(stderr)) == -1)
                printf("Error duplicating P2[1] (i=%d)\n", i);
            // close writing end of P2
            if (close(P2[1]) == -1)
                printf("Error closing P2[1] (i=%d)\n", i);

            // close the writing ends of all P1 pipes (not just the P1 for this process) and
            // close the reading ends of all P2 pipes (not just the P2 for this process):
            for (int z=0; z<4; z++) {
                if (close(fds[z][0][1]) == -1)
                    printf("Error closing P1[1] (i=%d)\n", i);
                if (close(fds[z][1][0]) == -1)
                    printf("Error closing P2[0] (i=%d)\n", i);
            }
            // now the child can read from parent with stdin, and write to parent with stderr

            errno = 0;
            // execute the candidate program (replaces this process with a new process image)
            // file location, program name, args (last arg must be NULL!)
            if (execlp("./candidate", "./candidate", argv[i+1], (char *)NULL) == -1) {
                // (only returns if there's an error)
                printf("failed starting candidate (i=%i). errno= %d\n", i, errno);
            }
            return 0;
        }
        // parent process:
        children[i] = pid;
    }
    // parent process:
    // close the reading end of all P1 pipes and the writing end of all P2 pipes
    for (int i=0; i<4; i++) {
        if (close(fds[i][0][0]) == -1)
            perror("Error closing a P0[0]: ");
        if (close(fds[i][1][1]) == -1)
            perror("Error closing a P1[1]: ");
    }

    // main menu
    while (1) {
        printf("Main Menu:\n");
        printf("  0. End program\n");
        printf("  1. Cast ballot\n");
        printf("  2. Show vote counts\n");
        printf("  3. Set vote count\n");
        printf("Choose an option>\n");

        char c;
        while (scanf("%c", &c) == EOF || c == '\n') {
            if (c != '\n')
                printf("Choose an option>\n");
        }

        switch (c) {
            case '0':
                // close each child's P1 writing end (tells them to terminate)
                for (int i=0; i<4; i++) {
                    if (close(fds[i][0][1]) != 0)
                        perror("Error closing a P1[1]: ");
                    // reap this child (wait for it to terminate)
                    waitpid(children[i], NULL, 0);
                }
                return 0; // exit program now that all children have terminated

            case '1':
                // print candidate names
                for (int i=0; i<4; i++) {
                    printf("  %d. %s\n", i, argv[i+1]);
                }
                printf("Choose a candidate>\n");
                char who;
                // clear stdin buffer (left over \n from last input
                while (scanf("%c", &who) == EOF || c == '\n');

                // wait for valid input
                //while (scanf("%c", &who) == EOF || c == '\n' || ! (c == '0' || c == '1' || c == '2')) {
                // TODO: validate input
                while (scanf("%c", &who) == EOF || c == '\n') {
                    printf("Choose a candidate>\n");
                }

                int child = who - '0';
                printf("child = %d\n", child);
                if (kill(children[child], SIGUSR1) == -1)
                    printf("failed to send SIGUSR1 to candidate %d\n", children[child]);
                break;

            case '2':
                // print vote counts
                for (int i=0; i<4; i++) {
                    if (kill(children[i], SIGUSR2) == -1)
                        printf("failed to send SIGUSR2 to candidate %d\n", children[i]);

                    // read number from pipe for this child
                    int* P2 = fds[i][1]; // P2 is for the child to write (to parent)
                    ///unsigned int count = 0;
                    char num[4];
                    if (read(P2[0], &num, 4) != 4)
                        printf("failed to read from candidate %d\n", children[i]);
                    printf("  %s has %x, %x, %x, %x (hex) votes\n", argv[i+1], num[0], num[1], num[2], num[3]);

                }
                break;
            case '3': // set vote count
                // print candidate names
                for (int i=0; i<4; i++) {
                    printf("  %d. %s\n", i, argv[i+1]);
                }
                printf("Choose a candidate>\n");
                // clear stdin buffer (left over \n from last input
                while (scanf("%c", &who) == EOF || c == '\n');

                // wait for valid input
                //while (scanf("%c", &who) == EOF || c == '\n' || ! (c == '0' || c == '1' || c == '2')) {
                // TODO: validate input
                while (scanf("%c", &who) == EOF || c == '\n') {
                    printf("Choose a candidate>\n");
                }

                int count;
                // clear stdin buffer (left over \n from last input
                char tmp;
                //while (scanf("%c", &tmp) == EOF || tmp == '\n');
                printf("Set vote count to?\n");
                // read number
                while (scanf("%d", &count) == EOF) {
                    printf("Set vote count to?\n");
                }

                child = who - '0';
                if (kill(children[child], SIGALRM) == -1)
                    printf("failed to send SIGALRM to candidate %d\n", children[child]);
                int* P1 = fds[child][0]; // P1 is for the child to read (from parent)
                if (write(P1[1], &count, sizeof(count)) != sizeof(count))
                    printf("failed to perform fraud on candidate %d\n", children[child]);
                
                break;
            default:
                printf("invalid input.\n");
        }
    }


    return 0;
}
