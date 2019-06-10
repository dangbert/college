#include <iostream>
using namespace std ;

#include "doctor.h"
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

   Character *morphed ;

   cout << "\n\n=====================================================================\n" ;
   cout << "p4test5.cpp:\n" ;
   cout << "Check that Doctor turns into Vampire after being bitten by a Vampire\n" ;
   cout << "=====================================================================\n" ;

   Vampire *v1 = new Vampire ;
   cout << "Create: " << v1->identify() << endl ;

   Doctor  *d1 = new Doctor ;
   cout << "Create: " <<d1->identify() << endl ;


   // Round 1
   // a vampire and a doctor walk into a bar

   cout << "*** Round 1\n" ;

   v1->encounter(d1) ;    // should bite
   d1->encounter(v1) ;

   expect(v1->migrates(), true, "Error: Vampire should migrate") ;
   expect(d1->migrates(), true, "Error: Doctor should migrate") ;

   expect(v1->morph(), NULL, "Bad morph: Vampire was not cured") ;
   expect(d1->morph() , NULL, "Bad morph: Doctor morphed early") ;

   // Round 2
   // vampire and doctor move to their own rooms

   cout << "*** Round 2\n" ;

   // no encounters

   expect(v1->migrates(), true, "Error: Vampire should migrate") ;
   expect(d1->migrates(), true, "Error: Doctor should migrate") ;

   expect(v1->morph(), NULL, "Bad morph: Vampire was not cured") ;
   expect(d1->morph() , NULL, "Bad morph: Doctor morphed early") ;


   // Round 3
   // vampire and doctor are in their own rooms

   cout << "*** Round 3\n" ;

   // no encounters

   expect(v1->migrates(), true, "Error: Vampire should migrate") ;
   expect(d1->migrates(), true, "Error: Doctor should migrate") ;

   expect(v1->morph(), NULL, "Bad morph: Vampire was not cured") ;
   expectNot(morphed = d1->morph() , NULL, "Bad morph: Doctor did not morph in Round 3") ;

   if (morphed != NULL) {

      cout << "Doctor morphed into: " << morphed->identify() << endl;

   } else {
      // do an extra round 
      expect(d1->migrates(), true, "Error: Doctor should migrate") ;
      expectNot(morphed = d1->morph() , NULL, "Bad morph: Doctor did not morph in Round 4 either") ;

      if (morphed != NULL)  cout << "Doctor morphed into: " << morphed->identify() << endl;
   }

   delete v1 ;
   delete d1 ;
   delete morphed ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
