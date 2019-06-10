#include "Tent.h"
using namespace std;

Tent::Tent() : Residence(1, 2 , false) {

}

double Tent::PropertyValue() const { //override the parent function
  return 0;
}

int Tent::NumWindows() const {
  //cout << "***Tent NumWindows called" << endl;
  return 0;
}


ostream& operator<<(ostream &sout, const Tent& t){ //must be overloaded to use tent's overloaded funtion
  //sout << "IN TENT OPERATOR:::" << endl;

  sout << "Number of Rooms: " << t.GetNumRooms() << endl;
  sout << "Number of Walls: " << t.GetNumWalls() << endl;
  sout << "Washer: ";
  if(t.HasWasher())
    sout << "true" << endl;
  else
    sout << "false" << endl;

  sout << "Number of windows: " << t.NumWindows() << endl;
  sout << "Property Value: " << t.PropertyValue() << endl;

}
