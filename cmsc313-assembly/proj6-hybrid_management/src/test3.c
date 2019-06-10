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
    printf("test3:\n");
    /* test 3 */

    init_heap();
    fraction* list[6];

    printf("After init_heap() called:\n");
    dump_heap();


    unsigned int i=0;
    for (i=0; i<6; i++) {
        list[i] = init_frac(-1, 2*i+1, 3*i);
    }
    printf("\n-----After creating 6 fractions:-----\n");
    dump_heap();


    printf("\n-----After deleting 3 fractions:-----\n");
    for (i=0; i<3; i++) {
        del_frac(list[i]);
    }
    dump_heap();


    for (i=1; i<3; i++) {
        list[i] = init_frac(-1, 2*i+1, 3*i);
    }
    printf("\n-----After creating 2 more fractions:-----\n");
    dump_heap();


    printf("\n-----After deleting all the fractions:-----\n");
    for (i=1; i<6; i++) {
        del_frac(list[i]);
    }
    dump_heap();


    return 0 ;
}
