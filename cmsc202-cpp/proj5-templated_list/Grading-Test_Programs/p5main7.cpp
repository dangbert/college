// File: p5main7.cpp
//
// Simple timing run to show that looping thru a LinkedList
// in reverse order takes quadratic time because we cannot 
// take advantage of caching the index.
//

#include <iostream>
#include <sys/times.h>
#include <sys/resource.h>
#include <stdexcept>

using namespace std ;

#include "list.h"


// Uses LinkedList [] operator and for loop that runs reps times.
//
void sumEvenSquares(unsigned int reps) {

   LinkedList<long int> L ;

   struct rusage time1, time2 ;
   long int elapsed ;
   bool failed ;

   for (unsigned int i = 0 ; i < reps ; i++) {
      L.addFront(i*i) ;
   }

   // record time before calls
   failed = getrusage(RUSAGE_SELF, &time1) ;
   if (failed) throw runtime_error("Could not get time") ;

   long int sum = 0 ;

   // Loop in reverse
   //
   for (int i = reps - 1 ; i >= 0 ; i--) {
      if (L[i] % 2 == 0) {
         sum += L[i] ; 
      }
   }

   // record time after calls
   failed = getrusage(RUSAGE_SELF, &time2) ;
   if (failed) throw runtime_error("Could not get time") ;

   // calculate elapsed time in microseconds
   elapsed = (time2.ru_utime.tv_sec - time1.ru_utime.tv_sec) * 1000000 ;
   elapsed += (time2.ru_utime.tv_usec - time1.ru_utime.tv_usec) ;
   
   
   cout << "Time for " << reps << " sumEvenSquares() calls took " << elapsed << " microseconds elapsed\n" ;
}


int main () {

   unsigned int base = 10000 ;     // adjust for your machine speed.

   // Running times should roughly quadruple each time
   sumEvenSquares(1*base);
   sumEvenSquares(2*base);
   sumEvenSquares(4*base);
   sumEvenSquares(8*base);

   cout << "Done!\n" ;

   return 0 ;
}
