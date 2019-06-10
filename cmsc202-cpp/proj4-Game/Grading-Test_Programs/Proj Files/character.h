#ifndef _CHARACTER_H_
#define _CHARACTER_H_

// Do not change any part of this file.

#include <string>
using namespace std ;

class Character {

public:   

   Character() ;
   virtual ~Character() ;

   // The encounter() function is called when the Game class
   // wants the host object to do something with the Character
   // specified by ptr. For example, if the host is a zombie
   // then ptr->biteMe() would be invoked.
   //
   virtual void encounter(Character *ptr) = 0 ;

   // biteMe() processes what happens when the host is bitten.
   virtual void biteMe(Character *ptr) = 0 ;

   // hitMe() processes what happens when the host is hit.
   virtual void hitMe() = 0 ;

   // cureMe() processes what happens when the host is cured.
   virtual void cureMe() = 0 ;

   // morph() returns a pointer to a Character that 
   // the host has turned into. For example, if the
   // host is human and is turning into a zombie this turn
   // then morph should return a pointer to a zombie.
   virtual Character *morph() = 0 ;

   // migrates() says whether the host moves around.
   virtual bool migrates() ;

   // Returns a string that describes the host
   // (e.g., "Human #6", "Vampire #7",...)
   virtual string identify() = 0 ;

   // returns m_id the unique ID number of the host
   unsigned int getId() ;

   // returns m_isDead. Note: Vampires and Zombies
   // are not dead --- they are undead.
   bool isDead() ;

protected:
   bool m_isDead ;

private:
   unsigned int m_id ;
   static unsigned int m_counter ;
} ;

#endif
