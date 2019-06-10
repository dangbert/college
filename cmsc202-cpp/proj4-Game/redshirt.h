#ifndef _REDSHIRT_H
#define _REDSHIRT_H

#include "character.h"

class RedShirt : public Character {

 public:  
  RedShirt();
  
  void biteMe(Character *ptr);
  void hitMe();
  void cureMe();
  string identify();
  void encounter(Character *ptr);
  bool migrates();
  Character* morph();
 
};

#endif
