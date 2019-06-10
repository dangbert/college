// File: p5main1.cpp
//
// Very simple test of [] operator

#include <iostream>
using namespace std ;

#include "list.h"

int main () {

   LinkedList<int> L ; 

   L.addFront(5) ;
   L.addFront(4) ;
   L.addFront(3) ;
   L.addFront(2) ;
   L.addFront(1) ;
   L.addFront(0) ;

   cout << "Print using [] operator\n" ;
   cout << L[0] << endl ;
   cout << L[1] << endl ;
   cout << L[2] << endl ;
   cout << L[3] << endl ;
   cout << L[4] << endl ;
   cout << L[5] << endl ;

   cout << "\nPrint using print function\n" ;
   L.print()  ;

   cout << "\nPrint using print function (second time)\n" ;
   L.print()  ;

   cout << "\nPrint using print function (third time)\n" ;
   L.print()  ;

   return 0 ;
}
