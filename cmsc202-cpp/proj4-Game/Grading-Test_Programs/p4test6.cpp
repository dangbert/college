#include <iostream>
using namespace std ;

#include "human.h"
#include "zombie.h"
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
   cout << "p4test6.cpp:\n" ;
   cout << "Check that Doctor can cure Human bitten by a Zombie\n" ;
   cout << "=====================================================================\n" ;

   Zombie *z1 = new Zombie ;
   cout << "Create: " << z1->identify() << endl ;

   Human  *h1 = new Human ;
   cout << "Create: " << h1->identify() << endl ;

   Doctor *d1 = new Doctor ;
   cout << "Create: " << d1->identify() << endl ;


   // Round 1
   // a zombie and a human walk into a bar
   // doctor alone in separate room

   cout << "*** Round 1\n" ;

   z1->encounter(h1) ;    // should bite
   h1->encounter(z1) ;

   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;
   expect(d1->migrates(), true, "Error: Doctors should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human morphed early") ;
   delete m ;
   expect(m = d1->morph() , NULL, "Bad morph: Doctor was not bitten") ;
   delete m ;


   // Round 2
   // zombie and human move to separate room
   // doctor move to same room as human

   cout << "*** Round 2\n" ;

   // doctor and human encounter each other
   d1->encounter(h1) ;   // should cure
   h1->encounter(d1) ;

   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;
   expect(d1->migrates(), true, "Error: Doctors should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human should be cured") ;
   delete m ;
   expect(m = d1->morph() , NULL, "Bad morph: Doctor was not bitten") ;
   delete m ;


   // Round 3
   // zombie, human and doctor are each in their own rooms

   cout << "*** Round 3\n" ;

   // no encounters

   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(h1->migrates(), true, "Error: Humans should migrate") ;
   expect(d1->migrates(), true, "Error: Doctors should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human should be cured") ;
   delete m ;
   expect(m = d1->morph() , NULL, "Bad morph: Doctor was not bitten") ;
   delete m ;


   // Round 4
   // zombie, human and doctor are each in their own rooms

   cout << "*** Round 4\n" ;

   // no encounters

   expect(z1->migrates(), true, "Error: Zombies should migrate") ;
   expect(h1->migrates(), true, "Humans should migrate") ;
   expect(d1->migrates(), true, "Doctors should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human should be cured") ;
   delete m ;
   expect(m = d1->morph() , NULL, "Bad morph: Doctor was not bitten") ;
   delete m ;


   // Round 5
   // zombie, human and doctor are each in their own rooms

   cout << "*** Round 5\n" ;

   // no encounters

   expect(z1->migrates(), true, "Zombies should migrate") ;
   expect(h1->migrates(), true, "Humans should migrate") ;
   expect(d1->migrates(), true, "Doctors should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human should be cured") ;
   delete m ;
   expect(m = d1->morph() , NULL, "Bad morph: Doctor was not bitten") ;
   delete m ;


   // Round 6
   // zombie, human and doctor are each in their own rooms

   cout << "*** Round 6\n" ;

   // no encounters

   expect(z1->migrates(), true, "Zombies should migrate") ;
   expect(h1->migrates(), true, "Humans should migrate") ;
   expect(d1->migrates(), true, "Doctors should migrate") ;

   expect(m = z1->morph(), NULL, "Bad morph: Zombie was not cured") ;
   delete m ;
   expect(m = h1->morph() , NULL, "Bad morph: Human should be cured") ;
   delete m ;
   expect(m = d1->morph() , NULL, "Bad morph: Doctor was not bitten") ;
   delete m ;


   // Clean up

   delete z1 ;
   delete h1 ;
   delete d1 ;

   cout << "\n\n=====================================================================\n\n\n" ;

   return 0;
}
