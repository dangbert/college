/* File: driver2.cpp

   UMBC CMSC 341 Fall 2016 Project 1

   Basic tests for the Llama class.

   This driver program tests Llama Stacks with floating 
   point numbers. 

*/

#include <iostream>
using namespace std ;

#include "Llama.h"

int main() {

   Llama<float,8> S ;
   float answer ;

   // Push a bunch of floats
  
   try {
      S.push(1.234) ;
      S.push(2.345) ;

      answer = S.pop() ;
      answer = S.pop() ;
      answer = S.pop() ;

   } catch (LlamaUnderflow &e) {
      cerr << "*****\n" ;
      cerr << "Llama Stack error: " << e.what() << endl ;
      cerr << "*****\n" ;
   }

   S.push(1.234) ;
   S.push(2.345) ;
   S.push(3.456) ;
   S.push(4.567) ;
   S.push(5.678) ;
   S.push(6.789) ;
   S.push(7.890) ;
   S.push(8.901) ;
   S.push(9.012) ;

   S.swap() ;
   S.dump() ;   

   S.push(10.123) ; 

   S.rot() ;
   S.dump() ;

   S.swap() ;
   S.dump() ;


   // Testing assignment and copy constructor
   //
   Llama<float,8> T(S) ;

   
   T.pop() ;
   T.pop() ;
   cerr << "============================\n" ;
   cerr << "Copied stack T has: \n" ;
   T.dump() ;
   cerr << "============================\n" ;
   cerr << "Orginal stack S has: \n" ;
   S.dump() ;


   Llama<float,8> U ;

   U = S ;
   U.pop() ;
   U.push(11.234) ;
   cerr << "============================\n" ;
   cerr << "Copied stack U has: \n" ;
   U.dump() ;
   cerr << "============================\n" ;
   cerr << "Orginal stack S has: \n" ;
   S.dump() ;

   return 0 ;
}
