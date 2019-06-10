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

   Character *morphed, *m ;

   cout << "\n\n=====================================================================\n" ;
   cout << "p4test1.cpp:\n" ;
   cout << "Check that Human turns into Zombie after being bitten by a Zombie\n" ;
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

   expect(z1->migrates(), true, "Zombies should migrate") ;
   expect(h1->migrates(), true, "Humans should migrate") ;

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
   expect(m = h1->morph() , NULL, "Bad morph: Human morphed early") ;
   delete m ;


   // Round 3
   // zombie and human are in their own rooms

   cout << "*** Round 3\n" ;

   // no encounters

   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expectNot(morphed = h1->morph() , NULL, "Bad morph: Human did not morph in Round 3") ;

   if (morphed != NULL) {

      cout << "Human morphed into: " << morphed->identify() << endl;

   } else {
      // do an extra round 
      expect(h1->migrates(), true, "Error: Humans should migrate") ;
      expectNot(morphed = h1->morph() , NULL, "Bad morph: Human did not morph in Round 4 either") ;

      if (morphed != NULL)  cout << "Human morphed into: " << morphed->identify() << endl;
   }

   delete z1 ;
   delete h1 ;
   delete morphed ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
