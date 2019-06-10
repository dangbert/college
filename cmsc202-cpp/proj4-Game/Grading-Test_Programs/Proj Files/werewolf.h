#ifndef _WEREWOLF_H_
#define _WEREWOLF_H_

#include "character.h"

class Werewolf : public Character {

public:

   Werewolf() ;
   void encounter(Character *ptr) ;
   void biteMe(Character *ptr) ;
   void hitMe() ;
   void cureMe() ;
   Character *morph() ;
   string identify() ;

private:
   unsigned m_timesHit ;
   bool m_wasCured ;

} ;

#endif
