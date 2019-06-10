/*Buffy hits. She does not bite. She is too quick for Zombies and Vampires, so
she cannot be bitten. She also cannot be hit. Curing has no effect on Buffy
since she doesn't get bitten. Buffy does not cure others. She does migrate.*/

#include <iostream>
#include <sstream>
#include "buffy.h"
using namespace std ;


Buffy::Buffy() {

}


void Buffy::biteMe(Character *ptr) { //after three turns the pointer passed (a new zombie) will enter the game 
  cout << "Missed me!" << endl;
  delete ptr;  
}


string Buffy::identify() { 
  ostringstream oss ;
  oss << "Buffy #" << getId() ;
  return oss.str() ;
}


void Buffy::encounter(Character *ptr) {
     cout << identify() << " hits " << ptr->identify() << endl;
    ptr->hitMe(); //hit the encountered character
}
