#ifndef _HUMAN_H
#define _HUMAN_H

#include "character.h"

class Human : public Character {

 public:
  Human();
  ~Human();
  void encounter(Character *ptr);
  void biteMe(Character *ptr);
  void hitMe();
  void cureMe();
  Character *morph();
  string identify();

 private:
  int m_infectedTurns; //number of turns finished since being bitten
  Character *m_ptr;   //pointer to a character that the host might morph into
};

#endif
