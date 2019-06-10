/**
 * @Author Dan Engbert (end1@umbc.edu)
 * This file contains a program that opens the files ioports and iomem
 * then display all lines whose address range encompasses the passed in value.
 * If no lines contain the device address, a message is displayed to indicate such.
 * If no device is given, or if the user gave multiple arguments,
 * or if a non-hexadecimal value is given, then an error message is dispalyed.
 *
 * Help received:
 *    https://wiki.osdev.org/I/O_Ports
 *    https://www.bogotobogo.com/Embedded/memory_mapped_io_vs_port_mapped_isolated_io.php
 *    https://stackoverflow.com/a/8393473/5500073
 *    https://stackoverflow.com/questions/2001626/fgets-function-in-c
 *    https://linux.die.net/man/3/strtoul
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#define _GNU_SOURCE // for getLine()

int main(int argc, char *argv[]) {
    // validate and parse args
    if (argc == 1) {
        fputs("No address given.\n", stdout);
        exit(1);
    }
    else if (argc > 2) {
        fputs("Multiple addresses given.\n", stdout);
        exit(1);
    }
    // verify arg is a hex number
    char val[strlen(argv[1] + 1)];              // + 1 for room for \0
    strcpy(val, argv[1]);
    char* tmp = val;
    // if string starts with 0x, skip first 2 chars (easier to check if it's hex)
    // NOTE: it would have been better to use the strtoul function to tell me if the string was valid
    if (strstr(val, "0x\0") == val || strstr(val, "0X\0") == val) {
        tmp += 2;
    }
    if (strlen(tmp) == 0 || tmp[strspn(tmp, "0123456789abcdefABCDEF")] != 0) {
        fputs("Invalid target address.\n", stdout);
        exit(1);
    }
    unsigned long int target = (unsigned long int)strtoul(argv[1], NULL, 16);

    // open /proc/ioports and /proc/iomem for reading
    FILE *fp;
    char str[1000];

    // iterate over files
    char *filenames[] = {"/proc/ioports", "/proc/iomem"};
    for (int i=0; i<2; i++) {
        if (i == 0) {
            fputs("ioports:\n", stdout);
        }
        else {
            fputs("iomem:\n", stdout);
        }

        // open file for reading
        fp = fopen(filenames[i] , "r");
        if (fp == NULL) {
            fputs("Unable to open file for reading.", stdout);
            exit(1);
        }

        int found = 0;                          // if we found at least one match
        // read file line by line
        while (fgets(str, 1000, fp) != NULL) {
            tmp = str;
            // advance across line until we are past all the whitespace
            while (*tmp != '\0') {
                if (tmp[0] != ' ') {
                    // we're at first non space

                    // get the first hex string
                    char *loc = strstr(tmp, "-\0"); // get index of '-'
                    int index = loc - tmp;

                    char num1[index+1];
                    memcpy(num1, &tmp[0], index);
                    num1[index] = '\0';
                    unsigned long int int1 = (unsigned long int)strtoul(num1, NULL, 16);

                    // get the second int
                    tmp = &tmp[index+1];
                    loc = strstr(tmp, " \0");       // look for next space
                    index = loc - tmp + 1;
                    char num2[index];
                    memcpy(num2, &tmp[0], index-1);
                    num2[index-1] = '\0';
                    unsigned long int int2 = (unsigned long int)strtoul(num2, NULL, 16);

                    // print line if address is in range
                    if (int1 <= target && target <= int2) {
                        // print this line because it encompasses the target address
                        fputs(str, stdout);
                        found += 1;
                    }
                    break;                      // continue to next line
                }
                tmp++;                          // advance one char forward on this line
            }
        }
        if (found == 0) {
            fputs("No devices found.\n", stdout);
        }
        fclose(fp);
    }

    return 0;
}
