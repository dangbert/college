#include <iostream>
using namespace std ;

#include "zombie.h"
#include "redshirt.h"


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
   cout << "p4test9.cpp:\n" ;
   cout << "Check Redshirts vs Zombies behavior\n" ;
   cout << "=====================================================================\n" ;

   RedShirt *r1 = new RedShirt ;
   RedShirt *r2 = new RedShirt ;
   RedShirt *r3 = new RedShirt ;
   RedShirt *r4 = new RedShirt ;
   RedShirt *r5 = new RedShirt ;
   cout << "Create: " << r1->identify() << endl ;
   cout << "Create: " << r2->identify() << endl ;
   cout << "Create: " << r3->identify() << endl ;
   cout << "Create: " << r4->identify() << endl ;
   cout << "Create: " << r5->identify() << endl ;

   Zombie *z1 = new Zombie ;
   cout << "Create: " << z1->identify() << endl ;


   // Round 1
   // RedShirt #1, #2 & #3 and Zombie in one room
   // RedShirt #4 & #5 together in a different room

   cout << "*** Round 1\n" ;

   z1->encounter(r1) ;    // should bite and kill RedShirt #1
   // no encounters for RedShirt #1
   r2->encounter(z1) ;    // hit #1
   r3->encounter(z1) ;    // hit #2

   r4->encounter(r5) ;   // friendly fire misses
   r5->encounter(r4) ;   // friendly fire misses


   expect(r1->migrates(), false, "RedShirts should not migrate") ;
   expect(r2->migrates(), false, "RedShirts should not migrate") ;
   expect(r3->migrates(), false, "RedShirts should not migrate") ;
   expect(r4->migrates(), false, "RedShirts should not migrate") ;
   expect(r5->migrates(), false, "RedShirts should not migrate") ;
   expect(z1->migrates(), true, "Zombies should migrate") ;

   expect(m = r1->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = r2->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = r3->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = r4->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = r5->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = z1->morph() , NULL, "Bad morph: Zombie not cured") ;
   delete m ;


   // Round 2
   // RedShirt #1 should be dead
   // RedShirt #2 and #3 stay in original room
   // Zombie wanders into room with RedShirt #4

   cout << "*** Round 2\n" ;

   if (r1->isDead()) {
      cout << r1->identify() << " is dead\n" ;
   } else {
      cout << "Error: " << r1->identify() << " is still alive\n" ;
   }

   z1->encounter(r4) ;   // should bite and kill RedShirt #4
   // no encounters for RedShirt #4
   r5->encounter(z1) ;   // 3rd hit on zombie
   
   r2->encounter(r3) ;   // friendly fire misses
   r3->encounter(r2) ;   // friendly fire misses


   expect(r2->migrates(), false, "RedShirts should not migrate") ;
   expect(r3->migrates(), false, "RedShirts should not migrate") ;
   expect(r4->migrates(), false, "RedShirts should not migrate") ;
   expect(r5->migrates(), false, "RedShirts should not migrate") ;
   expect(z1->migrates(), true, "Zombies should migrate") ;

   expect(m = r2->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = r3->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = r4->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = r5->morph(), NULL, "Bad morph: RedShirts do not morph") ;
   delete m ;
   expect(m = z1->morph() , NULL, "Bad morph: Zombie not cured") ;
   delete m ;


   // Round 3
   // RedShirts #2 & #3 in original room
   // RedShirt #4 should be dead
   // Zombie in separate room

   cout << "*** Round 3\n" ;

   // RedShirt #4 should be dead
   if (r4->isDead()) {
      cout << r4->identify() << " is dead\n" ;
   } else {
      cout << "Error: " << r4->identify() << " is still alive\n" ;
   }

   // Zombie should be dead
   if (z1->isDead()) {
      cout << z1->identify() << " is dead\n" ;
   } else {
      cout << "Error: " << z1->identify() << " is still alive\n" ;
   }

   // Skip rest of Round 3

   delete z1 ;
   delete r5 ;
   delete r4 ;
   delete r3 ;
   delete r2 ;
   delete r1 ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
