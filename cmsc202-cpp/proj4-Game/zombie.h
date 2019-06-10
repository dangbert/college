#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include "character.h"

class Zombie : public Character {

 public:
  Zombie();
  void encounter(Character *ptr);
  void biteMe(Character *ptr);
  void hitMe();
  void cureMe();
  Character *morph();
  string identify();

 private:
  int m_health; //the zombies health
  bool m_readyToMorph; //true when the zombie should morph at the end of a turn

};

#endif
