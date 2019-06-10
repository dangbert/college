/**
 * @Author Dan Engbert (end1@umbc.edu)
 * This file contains a program that unit tests the drivers implemented
 * in sha3.c
 * Focues on boundary tests.
 *
 * Help Received:
 * https://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/
 * https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxbd00/rtfil.htm
 * https://stackoverflow.com/a/32355825
 *
 * man pages: mmap, open, read, write, pread, pwrite
 */
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static unsigned test_passed;
static unsigned test_failed;
/*static void printBytes(char *buf, unsigned int size);*/
static void test0();
static void test1();
static void test2();
static void test3();
static void test4();
static void test5();

int data_fd;
int ctl_fd;

#define CHECK_IS_EQUAL(valA,valB) \
    do { \
        if ((valA) == (valB)) { \
            test_passed++; \
            printf("%d: PASS\n", __LINE__); \
        } else { \
            test_failed++; \
            printf("%d: FAIL\n", __LINE__); \
        } \
    } while(0);


int main(void)
{
    /* open files */
    data_fd = open("/dev/sha3_data", O_RDWR);
    ctl_fd = open("/dev/sha3_ctl", O_RDWR);

    /* perform (6) unit tests */
    test0(); /* must be run first */
    test1();
    test2();
    test3();
    test4();
    test5();

    /* close files */
    close(data_fd);
    close(ctl_fd);
    return 0;
}

/**
 * (helper function) prints all the bytes in an array
 */
/*
static void printBytes(char *buf, unsigned int size) {
    for (unsigned int i=0; i<size; i++) {
        if (i % 16 == 0)
            printf("\n");
        printf("%d ", (int) buf[i]);

    }
    printf("\n");
}
*/

/**
 * test that if user hasn't written to /dev/sha_ctl
 * then reading from it will return 0 bytes
 */
static void test0() {
    ssize_t res;
    char tmp_buf[PAGE_SIZE];
    printf("\ntest0: reading from /dev/sha3_ctl without ever calculating a hash\n");
    printf("\t(NOTE: Test only works if nothing wrote to /dev/sha3_ctl\n\tsince the driver module was inserted into the kernel!)\n");
    res = pread(ctl_fd, tmp_buf, 64, 0); /* try to read 64 bytes */
    CHECK_IS_EQUAL(res, 0);
}

/**
 * test that user can't read past PAGE_SIZE bytes in /dev/sha3_data
 */
static void test1() {
    ssize_t res;
    char tmp_buf[PAGE_SIZE+1]; /* buffer to store copy of data buffer */
    printf("\ntest1: reading past PAGE_SIZE\n");
    res = pread(data_fd, tmp_buf, PAGE_SIZE+1, 0); /* # of bytes read */
    CHECK_IS_EQUAL(res, PAGE_SIZE);
}

/**
 * test that if user attempts to write some bytes before position PAGE_SIZE
 * and some after, then only the bytes before it will be written.
 */
static void test2() {
    unsigned int i;
    ssize_t res;
    char tmp_buf[PAGE_SIZE];
    printf("\ntest2: writing some bytes past PAGE_SIZE in /dev/sha3_data\n");

    /* write entire buffer to all '9' first */
    memset(tmp_buf, 9, PAGE_SIZE * sizeof(tmp_buf[0]));
    res = pwrite(data_fd, tmp_buf, PAGE_SIZE, 0); /* write from offset 0 */
    CHECK_IS_EQUAL(res, PAGE_SIZE);

    /* now try to write some (different) data past PAGE_SIZE */
    memset(tmp_buf, 5, PAGE_SIZE * sizeof(tmp_buf[0]));
    i = (PAGE_SIZE-3);
    res = pwrite(data_fd, tmp_buf+i, 5, i); /* offset: PAGE_SIZE-3 */
    CHECK_IS_EQUAL(res, 3); /* 3 bytes should have been written */

    /* copy entire buf back into tmp_buf and check values */
    res = pread(data_fd, tmp_buf, PAGE_SIZE, 0);
    CHECK_IS_EQUAL(tmp_buf[i-1], 9);
    for ( ; i<PAGE_SIZE; i++) {
        CHECK_IS_EQUAL(tmp_buf[i], 5);
    }
    /*printBytes(tmp_buf, PAGE_SIZE);*/
}

/**
 *  test that if user attempts to write with offset >= PAGE_SIZE
 *  then 0 bytes will be written.
 */
static void test3() {
    ssize_t res;
    char tmp_buf[PAGE_SIZE];
    printf("\ntest3: writing only bytes past PAGE_SIZE in /dev/sha3_data\n");

    memset(tmp_buf, 1, PAGE_SIZE * sizeof(tmp_buf[0]));
    res = pwrite(data_fd, tmp_buf, 12, PAGE_SIZE);
    CHECK_IS_EQUAL(res, 0);
}

/**
 * test that user can't try to hash more than PAGE_SIZE bytes
 */
static void test4() {
    ssize_t res;
    char tmp_buf[PAGE_SIZE];
    char count_val[] = "4097";
    printf("\ntest4: hashing more than PAGE_SIZE bytes\n");

    /* write string to be hashed */
    memset(tmp_buf, 'E', PAGE_SIZE * sizeof(tmp_buf[0]));
    res = pwrite(data_fd, tmp_buf, PAGE_SIZE, 0);

    /* attempt to hash 4097 bytes */
    res = pwrite(ctl_fd, count_val, 4, 0);
    CHECK_IS_EQUAL(res, -1);
}

/**
 * test that user can't hash when they write a non-number to /dev/sha3_ctl
 */
static void test5() {
    ssize_t res;
    char tmp_buf[PAGE_SIZE];
    char count_val[] = "9hello2";
    printf("\ntest5: hashing a non-number of bytes\n");

    /* write string to be hashed */
    memset(tmp_buf, 'E', PAGE_SIZE * sizeof(tmp_buf[0]));
    res = pwrite(data_fd, tmp_buf, PAGE_SIZE, 0);

    /* attempt to hash (passing a non-number as the # of bytes */
    res = pwrite(ctl_fd, count_val, 7, 0);
    CHECK_IS_EQUAL(res, -1);
}
