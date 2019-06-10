#include <iostream>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Pinball.h"

using namespace std ;

// Words to add to the hash table
//
const char * inWords[10] = {
   "cauterising",
   "bursar",
   "outbursts",
   "glider",
   "intercity",
   "cartographers",
   "reggae",
   "chivvy",
   "merchantability",
   "felicitation"
} ;


// Words not in the hash table
//
const char * outWords[10] = {
   "misfiled",
   "opinionated",
   "benefactress",
   "hyperventilating",
   "staged",
   "proprietors",
   "foresee",
   "defrost",
   "spellbinder",
   "womenfolk"
} ;



int main() {

   int slot ;

   // change size to increase chance of collision
   Pinball PH(13) ;

   cout << "Insert some words\n" ;
   for (int i=0 ; i < 10 ; i++) {
      try {

         PH.insert(inWords[i]) ;

         // check if inserted word is in place
         //
         slot = PH.find(inWords[i]) ;
         if (strcmp(PH.at(slot),inWords[i]) != 0) {
            cout << "Inserted word not stored at slot = " << slot << endl ;    
            cout << "   inWords[" << i << "] = " << inWords[i] << endl ;
            cout << "   PH.at(" << slot << ") = " << PH.at(slot) << endl ;
         }

      } catch (PinballHashFull &e) {
         cout << e.what() << endl ;
         break ;
      } catch (...) {
         cout << "Unknown error\n" ;
      }
   }

   cout << "\n\n" ;
   PH.printStats() ;

   cout << "\n\n" ;
   cout << "Look for inserted words...\n" ;
   
   for (int i=0 ; i < 10 ; i++) {

      slot = PH.find(inWords[i]) ;
      if (slot < 0) {
         cout << "Inserted word not found: " << inWords[i] << endl  ;
      } else if (strcmp(PH.at(slot),inWords[i]) != 0) {
          cout << "Found word not equal!\n" ;    
      }

   }


   cout << "\n\n" ;
   cout << "Look for words not inserted (should fail)...\n" ;
   for (int i=0 ; i < 10 ; i++) {
      slot = PH.find(outWords[i]) ;
      if (slot > -1) {
         cout << "Found word not stored in hash table: " 
              << PH.at(slot) << endl ;
      }
   }


   // Remove two words from the hash table
   int remove1=2 ;
   int remove2=6 ;
  
   char *str ;
   cout << "\n\n" ;
   cout << "Removing " << inWords[remove1] << endl ;
   str = PH.remove(inWords[remove1]) ;
   if (strcmp(str,inWords[remove1]) != 0) {
      cout << "Wrong word removed!" << endl
           << "   str = " << str << endl 
           << "   inWords[" << remove1 << "] = " << inWords[remove1] << endl ;
   }
   free(str) ;

   cout << "Removing " << inWords[remove2] << endl ;
   str = PH.remove(inWords[remove2]) ;
   if (strcmp(str,inWords[remove2]) != 0) {
      cout << "Wrong word removed!" << endl
           << "   str = " << str << endl 
           << "   inWords[" << remove2 << "] = " << inWords[remove2] << endl ;
   }
   free(str) ;


   // Look for inserted words again. Two should be missing.
   //
   cout << "\n\n" ;
   cout << "Look for inserted words...\n" ;
   for (int i=0 ; i < 10 ; i++) {
      slot = PH.find(inWords[i]) ;
      if (slot < 0) {
         cout << "Inserted word not found: " << inWords[i] << endl  ;
      } else if (strcmp(PH.at(slot),inWords[i]) != 0) {
          cout << "Found word not equal!\n" ;    
      }
   }


   // Look for words definitely not in the table again.
   // 
   cout << "\n\n" ;
   cout << "Look for words not inserted (should fail)...\n" ;
   for (int i=0 ; i < 10 ; i++) {
      slot = PH.find(outWords[i]) ;
      if (slot > -1) {
         cout << "Found word not stored in hash table: " 
              << PH.at(slot) << endl ;
      }
   }


}

