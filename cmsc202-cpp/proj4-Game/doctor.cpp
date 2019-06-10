/*Doctors are like humans, except they cure. Doctors who are bitten cannot cure
themselves, but they can be cured by other Doctors. If a Doctor becomes a Zombie and
is subsequently cured, that Doctor/Zombie turns into a regular Human, not a Doctor.*/

#include <iostream>
#include <sstream>
#include "doctor.h"
using namespace std ;


Doctor::Doctor() {

}


string Doctor::identify() { 
  ostringstream oss ;
  oss << "Doctor #" << getId() ;
  return oss.str() ;
}


void Doctor::encounter(Character *ptr) {
    cout << identify() << " cures " << ptr->identify() << endl;
    ptr->cureMe(); //attempt to cure the encountered character
}
