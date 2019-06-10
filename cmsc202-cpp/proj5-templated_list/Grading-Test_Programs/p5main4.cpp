// File: p5main4.cpp
//
// Testing with list of vectors as 2D array.
// A LinkedList of vectors can be used with [][].
//

#include <iostream>
#include <vector>
using namespace std ;

#include "list.h"

int main () {

   LinkedList< vector<int> > A ;
   vector<int> dummy7 ;


   // Create a vector<int> with 7 items
   //
   for (unsigned int i = 0 ; i < 7 ; i++) {
      dummy7.push_back(0) ;
   }
   

   //  Add dummy7 to A 7 times
   //  Remember that addFront() makes a copy.
   //  So, A has 7 different copies of dummy7.
   //
   for (unsigned int i = 0 ; i < 7 ; i++) {
      A.addFront(dummy7) ;  // addFront() copies
   }


   // Construct mod 7 multiplication table
   //
   for (unsigned int i = 0 ; i < 7 ; i++) {
      for (unsigned int j = 0 ; j < 7 ; j++) {
         A[i][j] = ( i * j ) % 7 ;
      }
   }


   // Print out mod 7 multiplication table
   //
   for (unsigned int i = 0 ; i < 7 ; i++) {
      for (unsigned int j = 0 ; j < 7 ; j++) {
         cout << A[i][j] << " " ;
      }
      cout << endl ;
   }

   return 0 ;
}
