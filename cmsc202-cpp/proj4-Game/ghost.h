#ifndef _GHOST_H_
#define _GHOST_H_

#include "character.h"

class Ghost : public Character {

public:

   Ghost() ;
   void encounter(Character *ptr) ;
   void biteMe(Character *ptr) ;
   void hitMe() ;
   void cureMe() ;
   Character *morph() ;
   string identify() ;

} ;

#endif
