#ifndef _BUFFY_H
#define _BUFFY_H

#include "character.h"
#include "human.h"

class Buffy : public Human { //Buffy publicly derives from Human due to their similarities

 public:  
  Buffy();
  void encounter(Character *ptr);
  void biteMe(Character *ptr);
  string identify();
  //cureMe() doesn't need to be overided because Buffy can never be bitten
  
};

#endif
