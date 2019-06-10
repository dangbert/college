/*(Think Star Trek.) A RedShirt does not migrate. RedShirts stay in the same
room and hit characters that they encounter. RedShirts die immediately if they
are bitten, thus they cannot be cured (and they die quickly).*/

#include <iostream>
#include <sstream>
#include "redshirt.h"
using namespace std ;


RedShirt::RedShirt() {

}


void RedShirt::biteMe(Character *ptr) {
  m_isDead = true;
  delete ptr;
}


void RedShirt::hitMe() {
  //do nothing
}


void RedShirt::cureMe() {
  //cout << "redshirts cannot be cured" << endl;
}

string RedShirt::identify() { 
  ostringstream oss ;
  oss << "RedShirt #" << getId() ;
  return oss.str() ;
}

void RedShirt::encounter(Character *ptr) {
  //cout << "Boo! " << ptr->identify() << ", " << "I am " << identify() << ".\n" ;
  //if(!isDead() {
  cout << identify() << " hits " << ptr->identify() << endl;
  ptr->hitMe(); //attempt to cure the encountered character
    //}

}

bool RedShirt::migrates() {
  return false;
}


Character* RedShirt::morph() {
  return NULL; //never morph
}
