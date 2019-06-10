#include <iostream>
using namespace std ;

#include "zombie.h"
#include "vampire.h"
#include "doctor.h"


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
   cout << "p4test7.cpp:\n" ;
   cout << "Check that Doctor can cure a Zombie but not a Vampire\n" ;
   cout << "=====================================================================\n" ;

   Doctor *d1 = new Doctor ;
   cout << "Create: " << d1->identify() << endl ;

   Doctor *d2 = new Doctor ;
   cout << "Create: " << d2->identify() << endl ;

   Zombie *z1 = new Zombie ;
   cout << "Create: " << z1->identify() << endl ;

   Vampire  *v1 = new Vampire ;
   cout << "Create: " << v1->identify() << endl ;


   // Round 1
   // Zombie, Vampire and two Doctors all in same room

   cout << "*** Round 1\n" ;

   d1->encounter(z1) ;   // should cure zombie
   d2->encounter(v1) ;   // should not cure vampire
   z1->encounter(v1) ;
   v1->encounter(z1) ;

   expect(d1->migrates(), true, "Error: Doctors should migrate") ;
   expect(d2->migrates(), true, "Error: Doctors should migrate") ;
   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(v1->migrates(), true, "Error: Vampires should migrate") ;

   expect(m = d1->morph() , NULL, "Bad morph: Doctor #1 was not bitten") ;
   delete m ;
   expect(m = d2->morph() , NULL, "Bad morph: Doctor #2 was not bitten") ;
   delete m ;
   expect(m = v1->morph(), NULL, "Bad morph: Vampire was not cured") ;
   delete m ;
   expectNot(morphed = z1->morph(), NULL, "Bad morph: Zombie should have been cured") ;
   if (morphed != NULL) cout << "Zombie morphed into: " << morphed->identify() << endl ;


   // Clean up

   delete morphed ;
   delete v1 ;
   delete z1 ;
   delete d2 ;
   delete d1 ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
