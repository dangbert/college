// File: p5main5.cpp
//
// Simple timing run to show that addFront() is O(1) time.
//

#include <iostream>
#include <sys/times.h>
#include <sys/resource.h>
#include <stdexcept>

using namespace std ;

#include "list.h"


// call addFront reps many times
//
void addFrontTest(unsigned int reps) {
   LinkedList<double> dL ;

   struct rusage time1, time2 ;
   long int elapsed ;
   bool failed ;

   // record time before calls
   failed = getrusage(RUSAGE_SELF, &time1) ;
   if (failed) throw runtime_error("Could not get time") ;

   for (unsigned int i = 0 ; i < reps ; i++) {
      dL.addFront(i*i + 0.25) ;
   }

   // record time after calls
   failed = getrusage(RUSAGE_SELF, &time2) ;
   if (failed) throw runtime_error("Could not get time") ;

   // calculate elapsed time in microseconds
   elapsed = (time2.ru_utime.tv_sec - time1.ru_utime.tv_sec) * 1000000 ;
   elapsed += (time2.ru_utime.tv_usec - time1.ru_utime.tv_usec) ;

   cout << "Time for " << reps << " addFront() calls took " << elapsed << " microseconds elapsed\n" ;
}



int main () {
   int base = 1000000 ;    // change for your local machine speed

   // The elapsed time should roughly double each time
   addFrontTest(1*base) ;
   addFrontTest(2*base) ;
   addFrontTest(4*base) ;
   addFrontTest(8*base) ;

   cout << "Done!\n" ;

   return 0 ;
}

