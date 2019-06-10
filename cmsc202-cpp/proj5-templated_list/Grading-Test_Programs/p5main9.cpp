// File: p5main9.cpp
//
// Run the Sieve of Eratosthenes and results
// against standard implementation.

#include <iostream>
using namespace std ;

#include "list.h"
#include "list2.h"

void expect(int m, int n) {
   if (m == n) return ;

   cerr << "****** Error ******\n" ;
   cerr << "   Expected value = " << m << endl ; 
   cerr << "   Computed value = " << n << endl ; 
}


int main () {

   Answer::LinkedList<int> Primes2 ; 
   LinkedList<int> Primes ; 

   const int N = 100 ;
   int num, j ;

   for (int i=N ; i >=0 ; i--) {
      Primes2.addFront(i) ;
      Primes.addFront(i) ;
   }


   // 0 and 1 are not Prime
   expect(Primes2.deleteFront(), Primes.deleteFront()) ;   // remove 0
   expect(Primes2.deleteFront(), Primes.deleteFront()) ;   // remove 1


   // Run Sieve of Eratosthenes on submitted linked list implementation
   for (unsigned int i = 0 ; i < Primes.size() ; i++) {
      num = Primes[i] ;
      cout << "Removing multiples of " << num << endl ;
      j = num ;
      while (j*num <= N) {     
         Primes.remove(j*num) ;
         j++ ;
      }
   }

   // Run Sieve of Eratosthenes on standard linked list implementation
   for (unsigned int i = 0 ; i < Primes2.size() ; i++) {
      num = Primes2[i] ;
      // cout << "Removing multiples of " << num << endl ;
      j = num ;
      while (j*num <= N) {     
         Primes2.remove(j*num) ;
         j++ ;
      }
   }

   cout << "\nThe prime numbers less than " << N << endl ;
   Primes.print() ;

   // Check answer
   cout << "\nCheck list sizes:\n" ;
   expect(Primes2.size(), Primes.size()) ;

   cout << "\nFinal item by item check\n" ;
   unsigned int minSize = Primes2.size() < Primes.size() ? Primes2.size() : Primes.size() ;
   for (unsigned int i = 0 ; i < minSize ; i++) {
      cout << "Check item # " << i << endl ;
      expect(Primes2[i], Primes[i]) ;
   }




   return 0 ;
}
