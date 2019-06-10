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
   fp->sign = s ;
   fp->numerator = n ;
   fp->denominator = d ;

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
    /* test1:
     * tests out creating fractions and then removing them in a different order
     */
    init_heap() ;
    fraction *f1, *f2, *f3, *f4, *f5, *f6;

    f1 = init_frac(1, 1, 5);
    f2 = init_frac(1, 2, 3);

    f3 = show_sum(f1, f1);
    f4 = show_sum(f1, f2);
    printf("\n-----After creating 4 fractions:-----\n");
    dump_heap();

    printf("\n------After deleting these fractions (in a different order):-----\n");
    del_frac(f3);
    del_frac(f4);
    del_frac(f1);
    del_frac(f2);

    f1 = NULL;
    f2 = NULL;
    f3 = NULL;
    f4 = NULL;
    dump_heap();

    printf("\n----------------------------------------\n");
    printf("\n-----After creating 1 new fraction:-----\n");
    f1 = init_frac(-1, 1, 3);
    dump_heap();

    f2 = init_frac(1, 2, 9);
    f3 = init_frac(-1, 12, 2);
    f4 = show_sum(f3, f2);
    f5 = show_sum(f4, f1);
    f6 = show_sum(f4, f5);
    printf("\n-----After creating 5 more fractions:-----\n");
    dump_heap();


    del_frac(f3);
    del_frac(f4);
    del_frac(f1);
    printf("\n-----After deleting 3 fractions (in a different order):-----\n");
    dump_heap();

    del_frac(f2);
    del_frac(f5);
    del_frac(f6);
    printf("\n-----After deleting remaining fractions (in a different order):-----\n");
    dump_heap();


    return 0 ;
}
