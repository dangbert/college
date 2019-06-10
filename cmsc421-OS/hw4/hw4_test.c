/**
 * HW4 Unit Tests.
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

/* your code must define these functions */
extern void my_malloc_stats(void);
extern void *my_malloc(size_t size);
extern void my_free(void *ptr);

static unsigned int num_caught_faults;
static unsigned test_passed;
static unsigned test_failed;
static bool expecting_fault;

struct thread_arg {
	char thread_id;
	char *base;
};

static void report_test_results(void) {
	printf("%u tests passed, %u tests failed.\n", test_passed, test_failed);
}

static void fault_handler(int signum, siginfo_t *siginfo __attribute__((unused)), void *context __attribute__((unused))) {
	printf("Caught signal %d: %s!\n", signum, strsignal(signum));
	if (!expecting_fault) {
		exit(EXIT_FAILURE);
	}
	num_caught_faults++;
}

#define CHECK_IS_NOT_NULL(ptrA) \
	do { \
		if ((ptrA) != NULL) { \
			test_passed++; \
			printf("%d: PASS\n", __LINE__); \
		} else { \
			test_failed++; \
			printf("%d: FAIL\n", __LINE__); \
		} \
	} while(0);

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

static void *test_thread_alloc(void *data) {
	struct thread_arg *arg = data;
	char *t = my_malloc(16);
	CHECK_IS_NOT_NULL(t);
	t[0] = arg->thread_id;
	CHECK_IS_EQUAL((t == (arg->base + 64)) || (t == (arg->base + 128)), 1);
	return NULL;
}

void hw4_test(void) {
	if (atexit(report_test_results) < 0) {
		perror("atexit");
		exit(EXIT_FAILURE);
	}

	struct sigaction sa = {
		.sa_sigaction = fault_handler,
		.sa_flags = SA_SIGINFO,
	};
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGSEGV, &sa, &sa) < 0) {
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR1, &sa, &sa) < 0) {
		exit(EXIT_FAILURE);
	}

	printf("Test 1: Display initialized memory\n");
	my_malloc_stats();
	test_passed++;

	printf("Test 2: Simple allocations\n");
	char *m1, *m2;
	m1 = my_malloc(30);
	CHECK_IS_NOT_NULL(m1);
	memset(m1, 'A', 30);
	m2 = my_malloc(100);
	CHECK_IS_NOT_NULL(m2);
	memset(m2, 'B', 70);
	my_malloc_stats();

	printf("Test 3: Simple freeing\n");
	my_free(m1);
	char *m3 = my_malloc(15);
	CHECK_IS_EQUAL(m3, m2 + 128);
	memset(m3, 'C', 8);
	my_malloc_stats();

	printf("Test 4: Out of memory condition\n");
	errno = 0;
	m1 = my_malloc(800);
	CHECK_IS_EQUAL(m1, NULL);
	CHECK_IS_EQUAL(errno, ENOMEM);

	printf("Test 5: Double-free\n");
	my_free(m2);
	expecting_fault = true;
	my_free(m2);
	CHECK_IS_EQUAL(num_caught_faults, 1);
	expecting_fault = false;
	my_malloc_stats();

	printf("Test 6: Threaded first-fit allocation\n");
	pthread_t pth[2];
	struct thread_arg args[2] = {
		{.thread_id = 'D', .base = m3, },
		{.thread_id = 'E', .base = m3, },
	};
	pthread_create(pth, NULL, test_thread_alloc, args);
	pthread_create(pth + 1, NULL, test_thread_alloc, args + 1);
	pthread_join(pth[0], NULL);
	pthread_join(pth[1], NULL);
	my_malloc_stats();
}
