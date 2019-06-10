// File: mediumtest.cpp
// Version: 2016-11-29 15:45pm


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


// A "bug-proof" way to call rand() that preserves the state of the
// random seed between calls to rand().
// Set the random seed by passing a non-zero parameter.
//
int myRand(int seed=0) {

   // static local variables persist between calls
   static int savedSeed ;   

   if (seed) savedSeed = seed ;

   int othersSeed = rand() ;  // save other people's seed
   srand(savedSeed) ;         // restore my seed
   int result = rand() ;
   savedSeed = result ;       // save for next time
   srand(othersSeed) ;        // restore other people's
   return result ;
}

int main() {

   int slot ;
   int N = 1117 ;

   struct timeval tp;
   gettimeofday(&tp, NULL);
   long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
   printf("Random seed set to: %ld\n\n", ms) ;
   myRand(ms) ;

   // Uncomment to use same random seed each time
   //
   // myRand(8781035) ;

   Pinball PH ;
   int index ;
   int reps=750 ;

   cout << "Insert some words\n" ;

   int saveIndices[reps] ;  // array to remember which words were picked

   for (int i=0 ; i < reps ; i++) {
      saveIndices [i] = -1 ;
   }


   // Pick some words from global words[] array to insert in the hash table. 
   // Pick words in index 0 .. 9999
   //
   for (int i=0 ; i < reps ; i++) {
      try {

         index = myRand() % 10000 ;  // 0 .. 9999
         saveIndices[i] = index ;    // save which words were picked
         PH.insert(words[index]) ;

         // check that inserted words are really there
         //
         slot = PH.find(words[index]) ;
         if (strcmp(PH.at(slot),words[index]) != 0) {
            cout << "Inserted word not stored at slot = " << slot << endl ;    
            cout << "   words[" << index << "] = " << words[index] << endl ;
            cout << "   PH.at(" << slot << ") = " << PH.at(slot) << endl ;
         }

      } catch (PinballHashFull &e) {
         cout << e.what() << endl ;
	 cout << "   iteration = " << i << endl ;
	 cout << "   words[" << index << "] = " << words[index] << endl ;
         break ;
      } catch (...) {
         cout << "Unknown error\n" ;
      }
   }

   cout << "\n\n" ;
   PH.printStats() ;


   // Check again that words we inserted are actually in the table.
   //
   cout << "\n\n" ;
   cout << "Look for inserted words...\n" ;
   for (int i=0 ; i < reps ; i++) {

      index = saveIndices[i] ;  // recover words that were picked
      if (index < 0) continue ;  // maybe exception thrown?

      slot = PH.find(words[index]) ;
      if (slot < 0) {
         cout << "Inserted word not found: " << words[index] << endl  ;
      } else if (strcmp(PH.at(slot),words[index]) != 0) {
          cout << "Found word not equal!\n" ;    
      }
   }



   // Check some words that are definitely not in the hash table
   // Words randomly picked from global words[] array in the
   // range 15000 .. 24999.
   // 
   cout << "\n\n" ;
   cout << "Look for words not inserted (should fail)...\n" ;
   for (int i=0 ; i < reps ; i++) {
      index = ( (myRand() % 10000) + 15000 ) % numWords ;   // 15000 ..  24999
      slot = PH.find(words[index]) ;
      if (slot > -1) {
         cout << "Found word not stored in hash table: " 
              << PH.at(slot) <<  endl ;
         cout << "   index = " << index << endl ;
         cout << "   slot = " << slot << endl ;
      }
   }


   // Pick some words to be removed from the hash table
   //
   char *str ;
   cout << "\n\n" ;
   cout << "Remove some words ...\n" ;
   for (int j = 0 ; j < 20 ; j++) {

      index = saveIndices[myRand() % reps] ;
      if (index < 0) continue ;  // maybe exception thrown?

      str = PH.remove(words[index]) ;  // recover words that were picked
      cout << "Removing " << words[index] << endl ;

      if (str != NULL && strcmp(str,words[index]) != 0) {
         cout << "Wrong word removed!" << endl
              << "   str = " << str << endl 
              << "   words[" << index << "] = " 
                 << words[index] << endl ;
      }
      free(str) ;
   }

  
   // Look for words in the hash table again.
   // The words we removed should be missing.
   //
   cout << "\n\n" ;
   cout << "Look for inserted words...\n" ;
   for (int i=0 ; i < reps ; i++) {

      index = saveIndices[i] ;
      if (index < 0) continue ;

      slot = PH.find(words[index]) ;
      if (slot < 0) {
         cout << "Inserted word not found: " << words[index] << endl  ;
      } else if (strcmp(PH.at(slot),words[index]) != 0) {
          cout << "Found word not equal!\n" ;    
      }
   }


   // One more time, check words we know are not in the hash table.
   // None of these should be found.
   //
   cout << "\n\n" ;
   cout << "Look for words not inserted (should fail)...\n" ;
   for (int i=0 ; i < reps ; i++) {
      index = ( (myRand() % 10000) + 15000 ) % numWords ;    // 15000 to 24999
      slot = PH.find(words[index]) ;
      if (slot > -1) {
         cout << "Found word not stored in hash table: " 
              << PH.at(slot) << endl ;
      }
   }

}

