// File: p5main2.cpp
//
// Simple test with list of doubles.
// Checks copy constructor and assignment operator.
// 

#include <iostream>
using namespace std ;

#include "list.h"

int main () {

   LinkedList<double> doubleL ;

   doubleL.addFront(5.25) ;
   doubleL.addFront(4.25) ;
   doubleL.addFront(3.25) ;
   doubleL.addFront(2.25) ;
   doubleL.addFront(1.25) ;
   doubleL.addFront(0.25) ;

   cout << "*** doubleL ***" << endl ;
   doubleL.print() ;

   LinkedList<double> dL2(doubleL) ;

   cout << "*** dL2 ***" << endl ;
   dL2.print() ;

   doubleL[2] = 17.413 ;
   doubleL[4] = 997.111 ;
   cout << "*** doubleL changed ***" << endl ;
   doubleL.print() ;

   cout << "*** dL2 did not change ***" << endl ;
   dL2.print() ;

   doubleL = dL2 ;
   cout << "*** doubleL restored ***" << endl ;
   doubleL.print() ;


   return 0 ;
}
