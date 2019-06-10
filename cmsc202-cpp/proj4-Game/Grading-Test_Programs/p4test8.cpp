#include <iostream>
using namespace std ;

#include "zombie.h"
#include "vampire.h"
#include "buffy.h"


void expect(bool b1, bool b2, string mesg) {
   if (b1 != b2) cout << mesg << endl ;
}

void expect(Character *ptr1, Character *ptr2, string mesg) {
   if (ptr1 != ptr2) cout << mesg << endl ;
}

void expectNot(Character *ptr1, Character *ptr2, string mesg) {
   if (ptr1 == ptr2) cout << mesg << endl ;
}


int main() {  

   Character *m ;

   cout << "\n\n=====================================================================\n" ;
   cout << "p4test8.cpp:\n" ;
   cout << "Check that Zombies and Vampires die after getting hit 3 times.\n" ;
   cout << "=====================================================================\n" ;

   Buffy *b1 = new Buffy ;
   Buffy *b2 = new Buffy ;
   Buffy *b3 = new Buffy ;
   Buffy *b4 = new Buffy ;
   cout << "Create: " << b1->identify() << endl ;
   cout << "Create: " << b2->identify() << endl ;
   cout << "Create: " << b3->identify() << endl ;
   cout << "Create: " << b4->identify() << endl ;

   Zombie *z1 = new Zombie ;
   cout << "Create: " << z1->identify() << endl ;

   Vampire *v1 = new Vampire ;
   cout << "Create: " << v1->identify() << endl ;


   // Round 1
   // Buffy #1 with Zombie
   // Buffy #2 with Vampire
   // Buffy #3 and #4 by themselves in other rooms.

   cout << "*** Round 1\n" ;

   b1->encounter(z1) ;    // should hit
   z1->encounter(b1) ;    // tries to bite, but misses

   b2->encounter(v1) ;    // should hit
   v1->encounter(b2) ;    // tries to bite, but misses

   expect(b1->migrates(), true, "Error: Buffy should migrate") ;
   expect(b2->migrates(), true, "Error: Buffy should migrate") ;
   expect(b3->migrates(), true, "Error: Buffy should migrate") ;
   expect(b4->migrates(), true, "Error: Buffy should migrate") ;
   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(v1->migrates(), true, "Error: Vampires should migrate") ;

   expect(m = b1->morph(), NULL, "Bad morph: Buffy does not morph") ;
   delete m ;
   expect(m = b2->morph(), NULL, "Bad morph: Buffy does not morph") ;
   delete m ;
   expect(m = b3->morph(), NULL, "Bad morph: Buffy does not morph") ;
   delete m ;
   expect(m = b4->morph(), NULL, "Bad morph: Buffy does not morph") ;
   delete m ;
   expect(m = z1->morph() , NULL, "Bad morph: Zombie not cured") ;
   delete m ;
   expect(m = v1->morph() , NULL, "Bad morph: Vampires do not morph") ;
   delete m ;


   // Round 2
   // Buffy #1 and #3 in same room as zombie
   // Buffy #2 and #4 in same room as vampire 

   cout << "*** Round 2\n" ;

   b1->encounter(z1) ;   // 2nd hit on zombie
   b3->encounter(z1) ;   // 3rd hit on zombie
   z1->encounter(b1) ;   // tries to bite, but misses

   b2->encounter(v1) ;   // 2nd hit on Vampire
   b4->encounter(v1) ;   // 3rd hit on Vampire
   v1->encounter(b2) ;   // tries to bite, but misses

   expect(b1->migrates(), true, "Error: Buffy should migrate") ;
   expect(b2->migrates(), true, "Error: Buffy should migrate") ;
   expect(b3->migrates(), true, "Error: Buffy should migrate") ;
   expect(b4->migrates(), true, "Error: Buffy should migrate") ;
   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(v1->migrates(), true, "Error: Vampires should migrate") ;

   expect(m = b1->morph(), NULL, "Bad morph: Buffy does not morph") ;
   delete m ;
   expect(m = b2->morph(), NULL, "Bad morph: Buffy does not morph") ;
   delete m ;
   expect(m = b3->morph(), NULL, "Bad morph: Buffy does not morph") ;
   delete m ;
   expect(m = b4->morph(), NULL, "Bad morph: Buffy does not morph") ;
   delete m ;
   expect(m = z1->morph() , NULL, "Bad morph: Zombie not cured") ;
   delete m ;
   expect(m = v1->morph() , NULL, "Bad morph: Vampires do not morph") ;
   delete m ;


   // Round 3
   // zombie and human are in their own rooms

   cout << "*** Round 3\n" ;

   if (z1->isDead()) {
      cout << z1->identify() << " is dead\n" ;
   } else {
      cout << "Error: " << z1->identify() << " is still alive\n" ;
   }

   if (v1->isDead()) {
      cout << v1->identify() << " is dead\n" ;
   } else {
      cout << "Error: " << v1->identify() << " is still alive\n" ;
   }


   delete v1 ;
   delete z1 ;
   delete b4 ;
   delete b3 ;
   delete b2 ;
   delete b1 ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
