/* File: driver.cpp

   Main program used to test Pinball Hash Tables.

   version: 2016-12-02 15:40
      Alternate version of myRand() that uses rand48()
*/

#include <iostream>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "words.h"
#include "Pinball.h"

using namespace std ;


// Get amount of user time since the program began in milliseconds.
//
double getmsecs() {
   struct rusage use ; 
   struct timeval utime ;

   getrusage(RUSAGE_SELF, &use) ;
   utime = use.ru_utime ;
   return ((double) utime.tv_sec)*1000.0 +  ((double) utime.tv_usec)/1000.0 ;
}


// This version uses lrand48().
// Set the random seed by passing a non-zero parameter.
//
int myRand(int seed=0) {
   if (seed) srand48(seed) ;
   return lrand48() ;
}




// Exercise the Pinball Hash table with size slots
// inserting reps items.
//
void test(int size, int reps) {

   int index ;
   int slot ;
   double startTime, stopTime ;

   Pinball PH(size) ;

   startTime = getmsecs() ;

   // Insert reps randomly chosen items
   //
   for (int i=0 ; i < reps ; i++) {

      // don't choose items already in the hash table
      do { 
         index = myRand() % numWords ;
         slot = PH.find(words[index]) ;
      } while(slot > -1) ;
      try {

         PH.insert(words[index]) ;  // might throw exception


         // double check that inserted items were stored properly
         // for debugging.

         /*
         slot = PH.find(words[index]) ;
         if (slot >=0 && strcmp(PH.at(slot),words[index]) != 0) {
            cout << "Inserted word not stored at slot = " << slot << endl ;    
            cout << "   words[" << index << "] = " << words[index] << endl ;
            cout << "   PH.at(" << slot << ") = " << PH.at(slot) << endl ;
         }
         */

      } catch (PinballHashFull &e) {
         cout << e.what() << endl ;
         break ;
      } catch (...) {
         cout << "Unknown error\n" ;
      }
   }

   stopTime = getmsecs() ;
   printf("Elapsed time = %.3lf milliseconds\n", stopTime - startTime) ;

   PH.printStats() ;
}


int main() {

   // set random seed to wall clock time in milliseconds
   //
   struct timeval tp;
   gettimeofday(&tp, NULL);
   long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
   printf("Random seed set to: %ld\n\n", ms) ;
   myRand(ms) ;
   srand(ms*37) ;   // other uses

   // When debugging/developing, uncomment a line below and
   // use a fixed random seed.
   //
   // myRand(82170) ; srand(82170*37) ;
   // myRand(3170325890) ; srand(3170325890*37) ;
   // myRand(9132919912) ; srand(9132919912*37) ;

   test(5003,4000) ;   // >5,000 slots, ~80% full
   printf("\n\n") ;

   test(10037,8000) ;  // >10,000 slots, ~80% full
   printf("\n\n") ;

   test(20101,16000) ;  // >20,000 slots, ~80% full
   printf("\n\n") ;

   return 0 ;
}
