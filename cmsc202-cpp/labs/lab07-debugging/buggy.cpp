#include <iostream>
using namespace std ;

int main () {
   int A[10] ;
   int i, n=22 ;

   // Store squares in A[].
   //
   i = 0 ;
   while (i != n) {
      A[i/2] = i*i ;
      i += 2 ;
   }

   cout << "The number 10 squared is " << A[10] << endl ;
   cout << " n = " << n << endl ;
}
