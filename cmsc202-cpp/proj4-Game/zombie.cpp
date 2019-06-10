/*Zombies bite, they do not hit. If they are hit 3 times, then they die. Zombies migrate. Zombies 
do not cure. If a zombie is bitten, nothing happens. (Zombies do not become vampires.) If a 
zombie is cured, it turns into a human.*/

#include <iostream>
#include <sstream>
#include "zombie.h"
#include "human.h"
using namespace std ;


Zombie::Zombie() {
  m_health = 3;
  m_readyToMorph = false;
}


void Zombie::biteMe(Character *ptr) {
   delete ptr ; //if a zombie is bitten, do nothing. delete ptr to prevent memory leak
}


void Zombie::hitMe() {
  m_health--;

  if(m_health == 0)
    m_isDead = true; //indicate that the zombie has died
}


void Zombie::cureMe() {
  //cout << "Zombie #" << getId() << " cureMe called" << endl << "isDead()=" << isDead() << endl;
  if(!isDead()) { ///turn into a human with morph
    m_readyToMorph = true;

  }
}


Character *Zombie::morph() {
  if(m_readyToMorph && !isDead()) {
    Human *hum = new Human();
    cout << identify() << " turns into " << hum->identify() << endl;
    return hum; //return the human this zombie will morph into
  }  
  return NULL; //when no morph occured
}


string Zombie::identify() {
   ostringstream oss ;
   oss << "Zombie #" << getId() ;
   return oss.str() ;
}


void Zombie::encounter(Character *ptr) {
   if(!isDead()) { //make sure host isn't dead
     Zombie *zum = new Zombie();
     cout << identify() << " bites " << ptr->identify() << endl;
     ptr->biteMe(zum); //bite the encountered character and pass the zombie it will turn into later
   }
}
