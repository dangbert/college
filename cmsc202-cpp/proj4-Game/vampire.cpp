/*Vampires have the same attributes as Zombies except that they cannot be cured.
They do die after being hit three times (same as Zombies).*/

#include <iostream>
#include <sstream>
#include "vampire.h"
using namespace std ;


Vampire::Vampire() {

}


void Vampire::cureMe() {

}


Character *Vampire::morph() {
  return NULL; //never morph
}


string Vampire::identify() {
   ostringstream oss ;
   oss << "Vampire #" << getId() ;
   return oss.str() ;
}


void Vampire::encounter(Character *ptr) {
   if(!isDead()) { //make sure host isn't dead
     cout << identify() << " bites " <<ptr->identify() << endl;
     Vampire *vam = new Vampire();
     ptr->biteMe(vam); //bite the encountered character and pass the vampire it will turn into later
   }

}
