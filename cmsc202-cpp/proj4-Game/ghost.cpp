#include <iostream>
#include <sstream>
using namespace std ;

#include "ghost.h"

Ghost::Ghost() {
   // do nothing 
}

void Ghost::biteMe(Character *ptr) {
   // do nothing
   cout << "We cannot be bitten.\n" ;
   delete ptr ;   // Must do this or memory leak!
}

void Ghost::hitMe() {
   // do nothing
   cout << "We cannot be hit.\n" ;
}

void Ghost::cureMe() {
   // do nothing
   cout << "We do not get sick. No use for a cure.\n" ;
}

Character *Ghost::morph() {
   // do nothing
   cout << "We do not change into anything.\n" ;
   return NULL ;
}

string Ghost::identify() {
   ostringstream oss ;
   oss << "Ghost #" << getId() ;
   return oss.str() ;
}


void Ghost::encounter(Character *ptr) {
   cout << "Boo! " << ptr->identify() << ", " ;
   cout << "I am " << identify() << ".\n" ;
}
