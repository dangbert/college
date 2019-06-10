#ifndef _VAMPIRE_H
#define _VAMPIRE_H

#include "character.h"
#include "zombie.h"

class Vampire : public Zombie { //vampire publicly derives from zombie due to their similarities

 public:  
  Vampire();
  void cureMe();
  Character* morph();
  string identify();
  void encounter(Character *ptr);
  
};

#endif
