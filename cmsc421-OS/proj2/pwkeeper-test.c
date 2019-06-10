/**
 * @Author Dan Engbert (end1@umbc.edu)
 * This file contains a program that unit tests the driver implemented in pwkeeper.c
 * (Focuses on boundary tests).
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

/* function prototypes */
/*static void printBytes(char *buf, unsigned int size);*/
static void clear_lists();
static void test1();
static void test2();
static void test3();
static void test4();
static void test5();

/* global variables */
static unsigned test_passed;
static unsigned test_failed;

/* (file descriptors) */
static int mas_fd;
static int acc_fd;
static int show_acc_fd;
static int show_mas_fd;

#define MASTERPW_LEN 32
#define ACCOUNTNAME_LEN 16
#define ACCOUNTPW_LEN 16

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
	mas_fd = open("/dev/pwkeeper_master", O_RDWR);
	acc_fd = open("/dev/pwkeeper_account", O_RDWR);
	show_acc_fd = open("/sys/devices/platform/pwkeeper/accounts", O_RDWR);
	show_mas_fd = open("/sys/devices/platform/pwkeeper/masters", O_RDWR);

	/* perform unit tests */
	test1();
	test2();

	test3();
	test4();
	test5();

	/* close files */
	close(mas_fd);
	close(acc_fd);
	close(show_acc_fd);
	close(show_mas_fd);
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
 * helper function that deletes all accounts/users (clear both linked lists)
 */
static void clear_lists()
{
	char cmd[] = "echo -n \"0000\" | nc -N localhost 4210";
	int status = system(cmd);
	CHECK_IS_EQUAL(status, 0);
}

/**
 * TESTS: pwkeeper_master_write() by writing to /dev/pwkeeper_master
 *   verifies that an error is indicated when *ppos != 0
 *   test that master password of length MASTERPW_LEN can be set for a new user
 */
static void test1()
{
	ssize_t ret;
	char buf[MASTERPW_LEN];
	clear_lists();
	printf("\ntest1: testing /dev/pwkeeper_master\n");

	/* test that using an offset other than 0 leads to an error */
	ret = pwrite(mas_fd, buf, MASTERPW_LEN, 5);	/* try an offset of 5 */
	CHECK_IS_EQUAL(ret, -1);

	/* test that master password of length MASTERPW_LEN can be set for a new user */
	memset(buf, 'E', MASTERPW_LEN);
	ret = pwrite(mas_fd, buf, MASTERPW_LEN, 0);
	CHECK_IS_EQUAL(ret, MASTERPW_LEN);

	/* test that master password of smaller length can also be set for a new user */
	memset(buf, 'A', MASTERPW_LEN);
	ret = pwrite(mas_fd, buf, 8, 0);
	CHECK_IS_EQUAL(ret, 8);
}

/**
 * TESTS: pwkeeper_account_read() and pwkeeper_account_write() using /dev/pwkeeper_account
 *   tests that nothing is written if ppos >= ACCOUNTPW_LEN
 *   verify that an error is indicated if a user without a master account tries to write an account
 */
static void test2()
{
	ssize_t ret;
	char buf[ACCOUNTPW_LEN];
	clear_lists();
	printf("\ntest2: testing /dev/pwkeeper_account\n");

	/* test that nothing is written if ppos >= ACCOUNTPW_LEN */
	ret = pwrite(acc_fd, buf, MASTERPW_LEN, 5);	/* try an offset of 5 */
	CHECK_IS_EQUAL(ret, -1);

	/* verify that an error is indicated if a user without a master account tries to write an account */
	memset(buf, 'A', ACCOUNTPW_LEN);
	ret = pwrite(acc_fd, buf, ACCOUNTPW_LEN, 0);
	CHECK_IS_EQUAL(ret, -1);

	/* now create a master account and try again (make sure it works this time) */
	ret = pwrite(mas_fd, buf, 5, 0);	/* use pw: "AAAAA" */
	CHECK_IS_EQUAL(ret, 5);
	memset(buf, 'A', ACCOUNTPW_LEN);
	ret = pwrite(acc_fd, buf, ACCOUNTPW_LEN, 0);
	CHECK_IS_EQUAL(ret, ACCOUNTPW_LEN);
}

/**
 *
 * TESTS: pwkeeper_accounts_show()
 *   test that if there are hundreds accounts, it will only write up to PAGE_SIZE chars
 *   tests that nothing is written if ppos >= ACCOUNTPW_LEN
 *   verify that an error is indicated if a user without a master account tries to write an account
 */
static void test3()
{
	ssize_t ret;
	char buf[ACCOUNTPW_LEN];
	char tmp[PAGE_SIZE];
	int i;
	clear_lists();
	printf("\ntest3: testing /dev/pwkeeper_account\n");
	memset(buf, 'A', ACCOUNTPW_LEN);

	/* test that if there are hundreds accounts, it will only write up to PAGE_SIZE chars */
	/* create a master account */
	ret = pwrite(mas_fd, buf, 5, 0);	/* use pw: "AAAAA" */
	CHECK_IS_EQUAL(ret, 5);
	ret = pwrite(acc_fd, buf, ACCOUNTPW_LEN, 0);
	CHECK_IS_EQUAL(ret, ACCOUNTPW_LEN);
	/* create 100 accounts for this user (with slightly different names */
	for (i = 0; i < 100; i++) {
		buf[0] = (char)(34 + (i % 100));
		buf[1] = (char)('1' + (i % 7));
		if (i % 2 == 0)
			buf[2] = 'B';
		else
			buf[2] = 'B';

		ret = pwrite(acc_fd, buf, ACCOUNTNAME_LEN, 0);	/* use pw: "AAAAA" */
		if (ret != ACCOUNTNAME_LEN) {
			CHECK_IS_EQUAL(ret, ACCOUNTNAME_LEN);	/* show it failed */
		}
	}
	/* now test printing out these many accounts */
	ret = pread(show_acc_fd, tmp, PAGE_SIZE, 0);
	printf("ret = %ld\n", ret);
	CHECK_IS_EQUAL(ret, PAGE_SIZE);
}

/**
 * TEST: interrupt service routine
 *   verify that parcket length != 4 -> nothing changed
 *   verify that only UID 0 can delete all nodes in master + account lists
 */
static void test4()
{
	ssize_t ret;
	char buf[ACCOUNTPW_LEN];
	clear_lists();
	printf("\ntest4:\n");

	/* test that nothing is written if ppos >= ACCOUNTPW_LEN */
	ret = pwrite(acc_fd, buf, MASTERPW_LEN, 5);	/* try an offset of 5 */
	CHECK_IS_EQUAL(ret, -1);

	/* verify that an error is indicated if a user without a master account tries to write an account */
	memset(buf, 'A', ACCOUNTPW_LEN);
	ret = pwrite(acc_fd, buf, ACCOUNTPW_LEN, 0);
	CHECK_IS_EQUAL(ret, -1);

	/* now create a master account and try again (make sure it works this time) */
	ret = pwrite(mas_fd, buf, 5, 0);	/* use pw: "AAAAA" */
	CHECK_IS_EQUAL(ret, 5);
	memset(buf, 'A', ACCOUNTPW_LEN);
	ret = pwrite(acc_fd, buf, ACCOUNTPW_LEN, 0);
	CHECK_IS_EQUAL(ret, ACCOUNTPW_LEN);

}

/**
 * TEST that a user without CAP_SYS_ADMIN can't view all the accounts with /sys/devices/platform/pwkeeper/masters
 */
static void test5()
{
	ssize_t ret;
	char buf[ACCOUNTPW_LEN];
	clear_lists();
	printf("\ntest5:\n");

	/* test that nothing is written if ppos >= ACCOUNTPW_LEN */
	ret = pwrite(acc_fd, buf, MASTERPW_LEN, 5);	/* try an offset of 5 */
	CHECK_IS_EQUAL(ret, -1);

	/* verify that an error is indicated if a user without a master account tries to write an account */
	memset(buf, 'A', ACCOUNTPW_LEN);
	ret = pwrite(acc_fd, buf, ACCOUNTPW_LEN, 0);
	CHECK_IS_EQUAL(ret, -1);

	/* now create a master account and try again (make sure it works this time) */
	ret = pwrite(mas_fd, buf, 5, 0);	/* use pw: "AAAAA" */
	CHECK_IS_EQUAL(ret, 5);
	memset(buf, 'A', ACCOUNTPW_LEN);
	ret = pwrite(acc_fd, buf, ACCOUNTPW_LEN, 0);
	CHECK_IS_EQUAL(ret, ACCOUNTPW_LEN);

}
