#ifndef LAB3_H
#define LAB3_H

/* ChooseTwo(n): computes (n choose 2) = n * (n - 1) / 2

   Preconditions: 2 <= n <= 32768

   Postconditions: if n < 2 or n > 32768, returns -1 (invalid 
      argument); otherwise, returns (n * (n-1) ) / 2.          */

int ChooseTwo(int n);

/* Factorial(n): computes n! = n * (n-1) * (n-2) * ... 2 * 1

   Preconditions: 0 <= n <= 12

   Postconditions: if n < 0 or n > 12, returns -1 (invalid 
      argument); otherwise, returns n!.  By convention, 
      0! is 1.                                                 */

int Factorial(int n);

#endif
