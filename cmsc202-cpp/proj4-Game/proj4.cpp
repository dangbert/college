//my project test file
#include <iostream>
using namespace std ;

#include "character.h"
#include "ghost.h"
#include "human.h"
#include "zombie.h"
#include "doctor.h"
#include "buffy.h"
#include "redshirt.h"
#include "vampire.h"
#include "game.h"

int main() {  
  Zombie z1;
  Human h2;
  Doctor d3;
  Human h4;
  Zombie z5;
  Human h6;
  Buffy b7;
  Buffy b8;
  RedShirt r9;

  Character *ptr1;
  Character *ptr2;

  cout << "r9.migrates()=" << r9.migrates() << endl;  
  cout << "h2.migrates()=" << h2.migrates() << endl;  
  b7.encounter(&b8); //test what happens when buffy is hit


  ///Test of zombie encountering a human three times.
  ///Also tested with a vampire biting instead. And with a doctor being bitten instead
  cout << endl << "*************************" << "TESTING HUMAN biteMe() " << "*************************" << endl;
  for(int i=1; i<=3; i++) {
    cout << endl << "***" << z1.identify() << " about to encounter " << h2.identify() << " time #" << i << endl;
    ptr1 = &h2;
    z1.encounter(ptr1);

    cout << endl << "***" << h2.identify() << " morph() about to be called once." << endl;
    ptr2 = h2.morph();

    if(ptr2 != NULL)
      cout << h2.identify() << " has morphed into " << ptr2->identify() << endl;
    delete ptr2; //delete the dynamically allocated (created with new) character
  }


  cout << endl << endl << "*************************" << "TESTING ZOMBIE hitMe() " << "*************************" << endl; //also tested ZOMBIE hitMe()
  for(int i=1; i<=3; i++) {
    cout << endl << "***" << z1.identify() << " about to be hit time #" << i << endl;
    b7.encounter(&z1);
    //z1.encounter(&b7); //test what happens when buffy is bit
    cout << "z1.isDead()=" << z1.isDead() << endl;
  }//z1 is now dead

  
  cout << endl << endl << "*************************" << "TESTING DOCTOR healing infected human" << "*************************" << endl; //also tested when the human is a doctor
  cout << endl << "***" << z5.identify() << " about to encounter " << h4.identify() << " once." << endl;
  ///ptr1 = &h4;
  z5.encounter(&h4);

  cout << endl << "***" << h4.identify() << " morph() about to be called once." << endl;
  h4.morph();

  cout << endl << "***" << d3.identify() << " about to encounter " << h4.identify() << " once." << endl;
  d3.encounter(&h4);

  cout << endl << "***" << h4.identify() << " morph() about to be called once." << endl;
  h4.morph();


  cout << endl << endl << "*************************" << "TESTING DOCTOR healing a zombie" << "*************************" << endl;
  //Also tested with a doctor attempting to heal a vampire
  cout << endl << "***" << d3.identify() << " about to encounter " << z5.identify() << " once." << endl;
  d3.encounter(&z5);

  cout << endl << "***" << z5.identify() << " morph() about to be called once." << endl;
  ptr2 = z5.morph();

  if(ptr2 != NULL)
    cout << z5.identify() << " has morphed into " << ptr2->identify() << endl;
  delete ptr2; //delete the dynamically allocated (created with new) character 



}
