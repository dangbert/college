/*
 * Dan Engbert
 * CMSC 313
 * Spring 17
 */
#include <stdio.h>
#include <stdlib.h>
#include "frac_heap.h"


union fraction_block_u {
    union fraction_block_u *next;
    fraction frac;
};
typedef union fraction_block_u fraction_block;

static fraction_block *head;


void init_heap(void) {
    head = NULL;
}


fraction *new_frac(void) {
    if (head == NULL) {
        head = malloc(5 * sizeof(fraction_block));
        printf("called malloc(%d): returned %p\n", (5 * sizeof(fraction_block)), head);
        if (head == NULL) {
            printf("Error: Malloc returned NULL.\n");
            exit(0);
        }

        unsigned int i=0;
        for (i=0; i<4; i++) {
            head[i].next = &head[i+1];
        }
        head[5].next = NULL;
    }

    fraction *frac = &head->frac;
    head = head->next;
    return frac;
}


void del_frac(fraction *frac) {
    fraction_block *fbp;
    /* cast to fraction_block becaues it's really a fraction_block */
    fbp = (fraction_block *) frac;
    fbp->next = head;
    head = fbp;
}


void dump_heap(void) {
    printf("\n**** BEGIN HEAP DUMP ****\n");

    fraction_block *ptr = head;
    while (ptr != NULL) {
        printf("%p\n", ptr);
        ptr = ptr->next;
    }
    printf("**** END HEAP DUMP ****\n");
}
