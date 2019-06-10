/* File: driver3.cpp

   UMBC CMSC 341 Fall 2016 Project 1

   Basic tests for the Llama class.

   This driver program tests Llama stacks with 
   OvCoInt objects (Overly Complicated Integers).

*/

#include <iostream>
using namespace std ;

#include "Llama.h"
#include "OvCoInt.h"

int main() {

   Llama<OvCoInt,9> S ;
   OvCoInt answer ;

   // Push a bunch of ints
  
   S.push(OvCoInt(1)) ;  // using constructor
   S.push(2) ;	// using implicit type conversion
   S.push(3) ;
   S.push(4) ;
   S.push(5) ;
   S.push(6) ;
   S.push(7) ;
   S.push(8) ;
   S.push(9) ;

   S.swap() ;
   S.dump() ;   

   S.push(10) ; 

   S.rot() ;
   S.dump() ;

   S.swap() ;
   S.dump() ;


   // Testing assignment and copy constructor
   //

   OvCoInt::debugOn() ;

   cerr << "\nCreating new Llama Stack of OvCoInt T using S\n" ;
   Llama<OvCoInt,9> T(S) ;

   answer = T.pop() ;
   answer = T.pop() ;
   cerr << "============================\n" ;
   cerr << "Copied stack T has: \n" ;
   T.dump() ;
   cerr << "============================\n" ;
   cerr << "Orginal stack S has: \n" ;
   S.dump() ;


   cerr << "\nCreating new Llama Stack of OvCoInt U\n" ;
   Llama<OvCoInt,9> U ;

   cerr << "\nAssigning Llama Stack S to U\n" ;
   U = S ;
   cerr << "============================\n" ;
   cerr << "Copied stack U has: \n" ;
   U.dump() ;


   int n ; 
   n = U.pop() ;   // use implicit outward type conversion
   cerr << "Popped " << n << " from Llama Stack U\n" ;
   U.push(11) ;

   cerr << "============================\n" ;
   cerr << "After modifying U, orginal stack S has: \n" ;
   S.dump() ;

   return 0 ;
}
