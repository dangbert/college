/*
 *  File: main5.c
 *
 *  This file tests out the functions in frac_heap.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "frac_heap.h"

void print_frac (fraction *fptr) {

   if (fptr->sign < 0) printf("-") ;

   printf("%d/%d", fptr->numerator, fptr->denominator) ;

}

/*
 * Initialize a fraction
 */
fraction *init_frac (signed char s, unsigned int n, unsigned int d) {
   
   fraction *fp ;

   fp = new_frac() ;
   if (fp != NULL) {
       fp->sign = s ;
       fp->numerator = n ;
       fp->denominator = d ;
   }
   else {
        printf("new_frac() returned NULL\n");
   }

   return fp ;
}


int main() {
    /* test 3 */
    /* calls del_frac on a pointer that is outside of the memory allocated by frac_heap */

    fraction *f1, *f2;
    init_heap();

    printf("After init_heap() called:\n");
    dump_heap();

    f1 = init_frac(1, 3, 7);
    f2 = init_frac(-1, 14, 29);
    printf("\n-----After creating 2 fractions:-----\n");
    dump_heap();

    printf("\n-----About to intentionally call del_frac() on an invalid pointer:-----\n");
    del_frac(f1+20);
    printf("you shouldn't see this message (program should have already exited\n");

    return 0 ;
}
