#include <stdlib.h>
#include <stdio.h>
#include "frac_heap.h"

#define LENGTH 20
#define END LENGTH

/* large global array of fractions
 * static to give it file scope and ensure code in other files
 * can't access it directly
 */
static fraction fracs[LENGTH];

/* global variable for the beginning of the free block list */
static unsigned int first_free = 0;


/* must be called once by the program using your functions before calls to any
 * other functions are made. This allows you to set up any housekeeping needed
 * for your memory allocator. For example, this is when you can initialize your
 * linked list of free blocks.
 */
void init_heap() {
    unsigned int i=0;
    for (i=0; i<LENGTH; i++) {
        fracs[i].sign = 0;
        fracs[i].numerator = 0;
        /* stores the index of the next available free index in fracs */
        fracs[i].denominator = i+1;

        if (i == LENGTH) {
            fracs[i].denominator = END;
        }
    }

}


/* must return a pointer to fraction. This must be the address of an item in
 * your global array of fractions. It should be an item taken from the list of
 * free blocks. (Don't forget to remove it from the list of free blocks!)
 */
fraction* new_frac() {
    if (first_free == END) {
        printf("No more memory available!\n");
        return NULL;
    }

    fraction* f = &fracs[first_free];
    first_free = f->denominator;
    f->denominator = 0;
    return f;
}


/*  takes a pointer to fraction and adds that item to the free block list. The
 *  programmer using your functions promises to never use that item again,
 *  unless the item is given to her/him by a subsequent call to new_frac().
 */
void del_frac(fraction* f) {
    unsigned int index = f - &fracs[0]; /* index of this fraction in fracs */
    if (! (0 <= index && index <= LENGTH-1)) {
        printf("Invalid pointer in del_frac(), exiting program.\n");
        exit(0);
    }

    /*printf("DELETING:::  %d: %d  %d  %d\n\n", index, f->sign, f->numerator, f->denominator);*/
    f->sign = 0;
    f->numerator = 0;
    f->denominator = first_free;
    first_free = index;
}


/* for debugging/diagnostic purposes. It should print out the entire contents
 * of the global array and the head of the free block list. This allows you to
 * see how your memory allocator is working.
 */
void dump_heap() {
    printf("\n**** BEGIN HEAP DUMP ****\n");
    printf("first_free = %d\n\n", first_free);

    unsigned int i=0;
    for (i=0; i<LENGTH; i++) {
        fraction* f = &fracs[i];
        printf("  %d: %d  %d  %d\n", i, f->sign, f->numerator, f->denominator);
    }
    printf("**** END HEAP DUMP ****\n");
}
