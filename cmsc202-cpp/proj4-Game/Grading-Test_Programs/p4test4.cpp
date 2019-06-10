#include <iostream>
using namespace std ;

#include "human.h"
#include "zombie.h"


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
   cout << "p4test4.cpp:\n" ;
   cout << "Check for memory leak if game ends with Human bitten by Zombie\n" ;
   cout << "=====================================================================\n" ;

   Zombie *z1 = new Zombie ;
   cout << "Create: " << z1->identify() << endl ;

   Human  *h1 = new Human ;
   cout << "Create: " << h1->identify() << endl ;


   // Round 1
   // a zombie and a human walk into a bar

   cout << "*** Round 1\n" ;

   z1->encounter(h1) ;    // should bite
   h1->encounter(z1) ;

   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human morphed early") ;
   delete m ;


   // Round 2
   // zombie and human move to their own rooms

   cout << "*** Round 2\n" ;

   // no encounters

   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expect(h1->morph() , NULL, "Bad morph: Human morphed early") ;
   delete m ;

   cout << "*** Game ends\n" ;

   delete z1 ;
   delete h1 ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
