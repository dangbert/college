/* File: OvCoInt.cpp

   Implements the overly complicated integer class.
*/

#include <iostream>
#include <assert.h>
#include "OvCoInt.h"

using namespace std ;


bool OvCoInt::debug=false ;

OvCoInt::OvCoInt (int data) {
   
   if (debug) {
      cerr << "||||| OvCoInt constructor called, data = " 
           << data << endl ;
   }

   ptr = new int ;
   *ptr = data ;
}


OvCoInt::OvCoInt (const OvCoInt& other) {

   assert (other.ptr != NULL) ;
   
   if (debug) {
      cerr << "||||| OvCoInt copy constructor called, other = " 
           << *(other.ptr) << endl ;
   }

   ptr = new int ;
   *ptr = *(other.ptr) ;
}


OvCoInt::~OvCoInt() {
   
   if (debug) {
      cerr << "||||| OvCoInt destructor called\n" ;
   }

   delete ptr ;
   ptr = NULL ;
}


void OvCoInt::debugOn() {
   debug = true ;
}


void OvCoInt::debugOff() {
   debug = false ;
}


const OvCoInt& OvCoInt::operator=(const OvCoInt& rhs) {

   assert(ptr != NULL && rhs.ptr != NULL) ;

   if (debug) {
      cerr << "||||| OvCoInt assignment operator called, rhs = " 
           << *(rhs.ptr) << endl ;
   }

   if (this == &rhs) return *this ;
   *ptr = *(rhs.ptr) ;
   return *this ;
}


OvCoInt OvCoInt::operator+(const OvCoInt& rhs) {

   assert(ptr != NULL && rhs.ptr != NULL) ;

   return OvCoInt( *ptr + *(rhs.ptr) ) ;  
}


OvCoInt OvCoInt::operator-(const OvCoInt& rhs) {

   assert(ptr != NULL && rhs.ptr != NULL) ;

   return OvCoInt( *ptr - *(rhs.ptr) ) ;  
}


OvCoInt OvCoInt::operator*(const OvCoInt& rhs) {

   assert(ptr != NULL && rhs.ptr != NULL) ;

   return OvCoInt( *ptr * *(rhs.ptr) ) ;  
}


OvCoInt OvCoInt::operator/(const OvCoInt& rhs) {

   assert(ptr != NULL && rhs.ptr != NULL) ;

   return OvCoInt( *ptr / *(rhs.ptr) ) ;  
}


OvCoInt::operator int() {

   assert(ptr != NULL) ;

   return *ptr ;
}


ostream& operator<<(ostream& os, const OvCoInt& item) {

    assert(item.ptr != NULL) ;

    os << *(item.ptr) ;
    return os ;
}

