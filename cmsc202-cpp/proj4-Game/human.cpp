/*Humans do not bite, they do not hit. If they are bitten by a zombie or a vampire, then they
 turn into the corresponding creature AFTER 3 turns. Humans do not cure. Only good guys hit, 
so if a Human is hit, nothing happens. (We'll assume that Buffy would not really punch a Human.)
If a Human is cured after they were bitten, then the countdown clock for turning into a Vampire 
or a Zombie is reset to zero (and the countdown stops). A cured Human is still vulnerable to 
bites in the future. I.e., curing does not convey any immunity. Humans do migrate. */

#include <iostream>
#include <sstream>
#include "human.h"
using namespace std ;


Human::Human() {
  m_infectedTurns = -1; //number of turns finished since the human was bit. -1 if not bit yet
}


Human::~Human() {
  if(m_infectedTurns != 3 && m_infectedTurns != -1) //if m_ptr was passed a character but not put into play
    delete m_ptr;
}


void Human::biteMe(Character *ptr) {//after 3 turns the ptr passed (a new zombie or vampire) will enter the game 
  
  if(m_infectedTurns == -1) { //if not bit yet
    m_infectedTurns = 0; //indicates the host was bit this turn
    m_ptr = ptr; //store the pointer of the character to be morphed into later
    //cout << m_ptr->identify() << " PASSED TO " << identify() << endl;
  }
  else {
    delete ptr; ///delete pointer if the host has already been bit
    //cout << "This human has already been bit before!" << endl;
  }
}


void Human::hitMe() {

}


void Human::cureMe() {
  if(m_infectedTurns != -1) { //if bit earlier
    delete m_ptr;
    m_infectedTurns = -1; //cure the human
  }
  
}

 
Character *Human::morph() { //called at the end of each turn
  if(m_infectedTurns != -1) //if bitten at some point
    m_infectedTurns++; //increment the number of turns infected because a turn just ended

  if(m_infectedTurns == 3) {
    cout << identify() << " turns into " << m_ptr->identify() << endl;
    return m_ptr; //m_ptr doesn't need to be deleted now because its entering the game as a character
  }
  
  return NULL ;
}


string Human::identify() { 
  ostringstream oss ;
  oss << "Human #" << getId() ;
  return oss.str() ;
}


void Human::encounter(Character *ptr) {
  cout << identify() << " does nothing" << endl;
}
