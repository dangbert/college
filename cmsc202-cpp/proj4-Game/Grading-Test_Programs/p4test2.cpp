#include <iostream>
using namespace std ;

#include "human.h"
#include "vampire.h"


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
   cout << "p4test2.cpp:\n" ;
   cout << "Check that Human turns into Vampire after being bitten by a Vampire\n" ;
   cout << "=====================================================================\n" ;

   Vampire *v1 = new Vampire ;
   cout << "Create: " << v1->identify() << endl ;

   Human  *h1 = new Human ;
   cout << "Create: " << h1->identify() << endl ;


   // Round 1
   // a vampire and a human walk into a bar

   cout << "*** Round 1\n" ;

   v1->encounter(h1) ;    // should bite
   h1->encounter(v1) ;

   expect(v1->migrates(), true, "Error: Vampire should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = v1->morph(), NULL, "Bad morph: Vampire was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human morphed early") ;
   delete m ;

   // Round 2
   // vampire and human move to their own rooms

   cout << "*** Round 2\n" ;

   // no encounters

   expect(v1->migrates(), true, "Error: Vampire should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = v1->morph(), NULL, "Bad morph: Vampire was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human morphed early") ;
   delete m ;


   // Round 3
   // vampire and human are in their own rooms

   cout << "*** Round 3\n" ;

   // no encounters

   expect(v1->migrates(), true, "Error: Vampire should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = v1->morph(), NULL, "Bad morph: Vampire was not cured") ;
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

   delete v1 ;
   delete h1 ;
   delete morphed ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
