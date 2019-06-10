#ifndef FRAC_HEAP_H
#define FRAC_HEAP_H

/* creates a struct and associates the type "fraction" with it */
/* see http://www.embedded.com/electronics-blogs/programming-pointers/4024450/Tag-vs-Type-Names */
typedef struct {
    signed char sign;
    unsigned int numerator;
    unsigned int denominator;
} fraction;


/* function prototypes */
void init_heap();
fraction* new_frac();
void del_frac(fraction* f);
void dump_heap();

#endif
