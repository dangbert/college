#include <iostream>
#include <sstream>
using namespace std ;

#include "werewolf.h"
#include "human.h"


Werewolf::Werewolf() {
   m_timesHit = 0 ;   
   m_wasCured = false ;
}


void Werewolf::biteMe(Character *ptr) {
   // do nothing, werewolf don't turn into vampires
   delete ptr ;
}


void Werewolf::hitMe() {
   m_timesHit++ ;
   if (m_timesHit >= 3) {
      m_isDead = true ;
   }
}


void Werewolf::cureMe() {
   if (!m_isDead) m_wasCured = true ;
}


Character *Werewolf::morph() {
   if (m_wasCured) {
      Character *ptr = new Human() ;
      cout << identify() << " turns into " << ptr->identify() << endl ;
      return ptr ;
   } else {
      return NULL ;
   }
}


string Werewolf::identify() {

   ostringstream oss ;

   oss << "Werewolf #" << getId()  ;

   return oss.str()  ;
}


void Werewolf::encounter(Character *ptr) {
   cout << identify() << " bites " << ptr->identify() << endl ;
   ptr->biteMe(new Werewolf()) ;
}
