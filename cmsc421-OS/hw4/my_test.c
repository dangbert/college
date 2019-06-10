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

void test0(void);
void test1(void);
void test2(void);
void test3(void);
void *memThread(void *param);

struct thread_arg {
	char id;
    unsigned int request;
	char val;
    char *base;
};

/**
 * Be sure to test that your code handles threads by spawning
 * multiple threads, with each thread allocating and deallocating space.
 * Also write routines that intentionally corrupt memory canaries.
 */
void hw4_test(void) {
    test0();
    //test1();
    //test2();
    //test3();
}

void *memThread(void *param) {
    struct thread_arg *arg = (struct thread_arg *) param;
    arg->base = my_malloc(arg->request);
    if (arg->base != NULL) {
        printf("thread %d allocation of %d bytes '%c' [SUCCESS]\n", arg->id, arg->request, arg->val);
        memset(arg->base, arg->val, arg->request);
        // uncomment below to test corrupted canary
        //memset(arg->base + arg->request, '#', 1);
        //memset(arg->base + arg->request + 1, '@', 1);
    }
    else
        printf("thread %d allocation of %d bytes '%c' [FAILURE returned NULL]\n", arg->id, arg->request, arg->val);

    return NULL;
}

/**
 * test that code is thread safe
 */
void test0(void) {
    // threading tests:
    const int len = 9;
    pthread_t threads[len];
    struct thread_arg args[len];
    for (int i=0; i<len; i++) {
        args[i].id = i;
        args[i].val = '0' + i;
        args[i].request = 30 * i;
        pthread_create(&(threads[i]), NULL, memThread, &(args[i]));
    }

    // reap all the threads
    for (int i=0; i<len; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\n\nall threads done\n");
    my_malloc_stats();

    printf("\n\nfreeing memory:\n");
    // free memory
    for (int i=0; i<len; i++) {
        if (args[i].base != NULL)
            my_free(args[i].base);
    }
    my_malloc_stats();
}

void test1(void) {
    const int len = 5;
    char *tmp[len];
    for (int i=0; i<len; i++) {
        size_t size = 30*(i+1);
        printf("trying to allocate %d bytes\n", (int) size);
        tmp[i] = my_malloc(size);
        memset(tmp[i], 'A' + i, size);
        if (i==len/2 || i == len-1)
            my_malloc_stats();
    }
    printf("\n\nfreeing i=0:\n");
    my_free(tmp[0]);
    printf("freeing i=1:\n");
    my_free(tmp[1]);
    printf("allocating 64 bytes:\n");
    size_t size = 64;
    char *tmp2 = my_malloc(64);
    memset(tmp2, '$', 64);
    my_malloc_stats();


    printf("\n\nfreeing all:\n");
    my_free(tmp[2]);
    my_free(tmp[3]);
    my_free(tmp[4]);
    my_free(tmp2);
    my_malloc_stats();

    printf("\nbig allocation:\n");
    size = 638;
    tmp2 = my_malloc(size);
    printf("tmp2 is %p\n", tmp2);
    memset(tmp2, 'Z', size);
    my_malloc_stats();
}

/**
 * test that first fit wraps around and always finds the right spot
 */
void test2(void) {
    const int len = 20;
    char *tmp[len];
    for (int i=0; i<len; i++) {
        size_t size = 62;
        if (i == 4)
            size = 120;
        if (i == 9)
            size = 64*3 - 2;
        printf("\n%d: trying to allocate %d bytes\n", i, (int) size);
        tmp[i] = my_malloc(size);
        if (tmp[i] == NULL) {
            printf("%d: RETURNED NULL\n", i);
            continue;
        }

        memset(tmp[i], 'A' + i, size);
        if (i == 2 || i == 5 || i == 6 || i == 7) {
            memset(tmp[i], '*', size);
            printf("(freeing it right away)\n");
            my_free(tmp[i]);
        }
        my_malloc_stats();
    }
    printf("\n\nfinal state:\n");
    my_malloc_stats();
}

/**
 * test that we can allocate up to 638 bytes
 */
void test3(void) {
    for (int i=0; i<2; i++) {
        printf("\n\n--------------:\n");
        size_t size = 64;
        char *c = my_malloc(size);
        if (c == NULL) {
            printf("my_malloc returned NULL for size %d\n", (int) size);
        }
        else {
            printf("success!\n");
        }
        my_malloc_stats();
        my_free(c);
        my_malloc_stats();
    }
}
