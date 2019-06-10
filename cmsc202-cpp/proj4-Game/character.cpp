// Do not change any part of this file

#include <iostream>
using namespace std ;

#include "character.h"

// initial static ID counter
unsigned int Character::m_counter = 0 ;

Character::Character() {

   // set unique id number
   m_counter++ ;
   m_id = m_counter ;

   // death is not default
   m_isDead = false ;
}


Character::~Character() { // virtual
   // do nothing
   // cout << "Character destructor called.\n" ;
}


bool Character::migrates() {  // virtual
   return true ;
}

unsigned int Character::getId() {
   return  m_id ; 
}

bool Character::isDead() {
   return  m_isDead ; 
}

