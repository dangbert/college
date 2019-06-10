#include <iostream>
using namespace std ;

#include "human.h"
#include "werewolf.h"


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
   cout << "p4test3.cpp:\n" ;
   cout << "Check that Human turns into Werewolf after being bitten by a Werewolf\n" ;
   cout << "=====================================================================\n" ;

   Werewolf *w1 = new Werewolf ;
   cout << "Create: " << w1->identify() << endl ;

   Human  *h1 = new Human ;
   cout << "Create: " << h1->identify() << endl ;


   // Round 1
   // a werewolf and a human walk into a bar

   cout << "*** Round 1\n" ;

   w1->encounter(h1) ;    // should bite
   h1->encounter(w1) ;

   expect(w1->migrates(), true, "Error: Werewolf should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = w1->morph(), NULL, "Bad morph: Werewolf was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human morphed early") ;
   delete m ;


   // Round 2
   // werewolf and human move to their own rooms

   cout << "*** Round 2\n" ;

   // no encounters

   expect(w1->migrates(), true, "Error: Werewolf should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = w1->morph(), NULL, "Bad morph: Werewolf was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human morphed early") ;
   delete m ;


   // Round 3
   // werewolf and human are in their own rooms

   cout << "*** Round 3\n" ;

   // no encounters

   expect(w1->migrates(), true, "Error: Werewolf should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;

   expect(m = w1->morph(), NULL, "Bad morph: Werewolf was not cured") ;
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

   delete w1 ;
   delete h1 ;
   delete morphed ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
