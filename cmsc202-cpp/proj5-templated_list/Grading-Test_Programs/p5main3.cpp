// File: p5main3.cpp
//
// Simple test of remove() and deleteFront()
// Simple test to verify cache invalidation
//
//

#include <iostream>
using namespace std ;

#include "list.h"

int main () {

   LinkedList<int> L ;

   // initialize list
   //
   for (unsigned int i = 0 ; i < 100 ; i++) {
      L.addFront(i*i - 2 * i + 4) ;
   }

   cout << "*** Original list: ***\n" ;
   L.print() ;
   cout << endl ;

   // Simple remove tests
   //
   if (L.remove(7399)) {
     cout << "Successfully removed 7399\n" ;
   } else {
     cout << "Did not find 7399\n" ;
   }
   cout << endl ;

   if (L.remove(4907)) {
     cout << "Successfully removed 4907\n" ;
   } else {
     cout << "Did not find 4907\n" ;
   }


   unsigned int s = L.size() ;
   int data ;

   cout << "\n*** Deleting from the front...\n" ;
   for (unsigned int i = 0 ; i < s ; i++) {
      data = L.deleteFront() ;
      cout << "Deleted " << data << endl ;
   }

   cout << "\nL.size() = " << L.size() << endl ;


   // Check cache invalidation works

   cout << "\n*** Check cache invalidation ***\n" ;
   LinkedList<int> L2 ;

   L2.addFront(9) ;
   L2.addFront(8) ;
   L2.addFront(7) ;
   L2.addFront(6) ;
   L2.addFront(5) ;
   L2.addFront(4) ;
   L2.addFront(3) ;
   L2.addFront(2) ;
   L2.addFront(1) ;
   L2.addFront(0) ;

   // normal run
   //
   for (unsigned int i = 0 ; i < L2.size() ; i++) {
      cout << L2[i] << " " ; 
   }
   cout << endl ; 

   // addFront called in the middle of the for loop.
   //
   for (unsigned int i = 0 ; i < L2.size() ; i++) {
      if (i == 3) L2.addFront(101) ;
      cout << L2[i] << " " ; 
   }
   cout << endl ; 

   // deleteFront called in the middle of the for loop.
   //
   for (unsigned int i = 0 ; i < L2.size() ; i++) {
      if (i == 3) L2.deleteFront() ;
      cout << L2[i] << " " ; 
   }
   cout << endl ; 

   // Another normal run
   //
   for (unsigned int i = 0 ; i < L2.size() ; i++) {
      cout << L2[i] << " " ; 
   }
   cout << endl ; 

   // remove called in the middle of the for loop.
   //
   for (unsigned int i = 0 ; i < L2.size() ; i++) {
      if (i == 3) L2.remove(7) ;
      cout << L2[i] << " " ; 
   }
   cout << endl ; 

   return 0 ;
}
