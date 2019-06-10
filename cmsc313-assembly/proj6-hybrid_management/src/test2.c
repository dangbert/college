/*
 *  File: main5.c
 *
 *  This file tests out the functions in frac_heap.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "frac_heap.h"

/*
 * Compute the greatest common divisor using Euclid's algorithm
 */
unsigned int gcd ( unsigned int a, unsigned int b) {

   if (b == 0) return a ;

   return gcd (b, a % b) ;
}

/*
 * Print a fraction out nicely
 */
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

/*
 * Add two fractions
 * Return value is a pointer to allocated space.
 * This must be deallocated using del_frac().
 */
fraction *add_frac(fraction *fptr1, fraction *fptr2) {
   unsigned int lcm, div, g, m1, m2  ;
   fraction *answer ;


   g = gcd(fptr1->denominator, fptr2->denominator) ;
   lcm = (fptr1->denominator / g) * fptr2->denominator ;

   m1 = (fptr1->denominator / g) ;
   m2 = (fptr2->denominator / g) ;
   lcm = m1 * fptr2->denominator ;

   answer = new_frac() ;
   answer->denominator = lcm ;

   if (fptr1->sign == fptr2->sign) {

      answer->sign = fptr1->sign ;
      answer->numerator = fptr1->numerator * m2 + fptr2->numerator * m1 ;

   } else if (fptr1->numerator >= fptr2->numerator) {

      answer->sign = fptr1->sign ;
      answer->numerator = fptr1->numerator * m2 - fptr2->numerator * m1 ;

   } else {

      answer->sign = fptr2->sign ;
      answer->numerator = fptr2->numerator * m2 - fptr1->numerator * m1 ;

   }

   div = gcd(answer->numerator, answer->denominator) ;
   answer->numerator /= div ;
   answer->denominator /= div ;

   return answer ;

}

fraction* show_sum(fraction *f1, fraction *f2) {
    fraction* f3 = add_frac(f1, f2);
    print_frac(f1) ; 
    printf(" + ") ;
    print_frac(f2) ; 
    printf(" = ") ;
    print_frac(f3) ; 
    printf("\n") ;
    return f3;
}


int main() {
    /* test 2:
     * tests out what happens if 20 fractions are created and then 1 more is
     * attempted to be created. Also tests out creating and deleting of
     * fractions.
     */
    printf("test2:\n");
    fraction* list[20];

    init_heap();
    printf("After init_heap() just called:\n");
    dump_heap();
    printf("\n");

    unsigned int i=0;
    for (i=0; i<20; i++) {
        list[i] = init_frac(-1, 2*i+1, 3*i);
    }
    printf("\n-----After creating 20 fractions:-----\n");
    dump_heap();


    for (i=0; i<3; i++) {
        del_frac(list[i]);
    }
    printf("\n-----After deleting the first 3 fractions:-----\n");
    dump_heap();

    for (i=3; i<3+4; i++) {
        del_frac(list[i]);
    }
    printf("\n-----After deleting 4 more fractions:-----\n");
    dump_heap();

    for (i=0; i<5; i++) {
        list[i] = init_frac(1, i, i);
    }
    printf("\n-----After creating 5 more fractions:-----\n");
    dump_heap();


    for (i=5; i<7; i++) {
        list[i] = init_frac(1, i, i);
    }
    fraction *f1 = init_frac(1, 7, 8);;
    printf("\n-----After creating 3 more fractions:-----\n");
    dump_heap();



    for (i=0; i<20; i++) {
        del_frac(list[i]);
    }
    del_frac(f1);
    printf("\n-----After deleting all (21) fractions:-----\n");
    dump_heap();

    return 0 ;
}
