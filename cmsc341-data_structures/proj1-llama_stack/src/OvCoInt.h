#ifndef _OVCOINT_H_
#define _OVCOINT_H_

/* File: OvCoInt.h

   OvCoInt is an overly complicated integer class.

*/

#include <iostream>
using namespace std ;

class OvCoInt {

   public:
   
   OvCoInt(int data=0) ;
   OvCoInt(const OvCoInt& other) ;
   ~OvCoInt() ;

   static bool debug ;
   static void debugOn() ;
   static void debugOff() ;
 
   const OvCoInt& operator=(const OvCoInt& rhs) ;

   OvCoInt operator+(const OvCoInt& rhs) ;
   OvCoInt operator-(const OvCoInt& rhs) ;
   OvCoInt operator*(const OvCoInt& rhs) ;
   OvCoInt operator/(const OvCoInt& rhs) ;

   operator int() ;  // implicit converstion to int

   friend ostream& operator<<(ostream& os, const OvCoInt& item) ;


   private:

   int *ptr ;

} ;

ostream& operator<<(ostream& os, const OvCoInt& item) ;

#endif
