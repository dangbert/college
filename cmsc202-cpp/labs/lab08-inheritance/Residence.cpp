#include <iostream>
#include "Residence.h"
using namespace std;

Residence::Residence(int nR, int nW, bool hW){
  numRooms = nR;
  numWalls = nW;
  hasWasher = hW;
}

bool Residence::HasWasher() const {
  return hasWasher;
  }

double Residence::PropertyValue() const {
  //cout << "***Residence's PropertyValue() called" << endl;
  return numRooms * 1000;
  }

int Residence::NumWindows() const {
  //cout << "***Residence's NumWindow() called" << endl;
  return numWalls * 2;
  }

int Residence::GetNumRooms() const{
  return numRooms;
}

int Residence::GetNumWalls() const{
  return numWalls;
}


ostream& operator<<(ostream &sout, const Residence& r){ //friend function (not inherited)
  //sout << "IN RESIDENCE OPERATOR:::" << endl;

  sout << "Number of Rooms: " << r.numRooms << endl;
  sout << "Number of Walls: " << r.numWalls << endl;
  sout << "Washer: ";
  if(r.hasWasher)
    sout << "true" << endl;
  else
    sout <<"false" << endl;

  sout << "Number of windows : " << r.NumWindows() << endl;
  sout << "Property Value: " << r.PropertyValue() << endl;
}



