#ifndef _DOCTOR_H
#define _DOCTOR_H

#include "character.h"
#include "human.h"

class Doctor : public Human { //doctor public derives from human

 public:  
  Doctor();
  void encounter(Character *ptr); //override the Human encounter and identify functions
  string identify();

};

#endif
