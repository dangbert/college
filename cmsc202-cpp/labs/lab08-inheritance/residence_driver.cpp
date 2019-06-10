#include<iostream>

#include "House.h"
#include "Apartment.h"
#include "Tent.h"
//#include "Residence.h"

using namespace std;

int main() {
  cout << "House: ";
  //make a House object
  House myHouse;  // House only has default constructor

  //use the overloaded "<<" operator to print the values related to this house
  cout << endl << myHouse << endl;

  cout << "Apartment: ";
  //make a new Apartment object on the 2nd floor that has a washer
  Apartment myApartment(2, true); //story, washer

  //use the overloaded "<<" operator to print the values related to
  //this apartment
  cout << endl << myApartment << endl;

  cout << "Tent: ";
  //make a new Tent object
  Tent myTent;
  //cout << "printing myTent.NumWindows():  " << myTent.NumWindows() << endl <<endl;

  //use the overloaded "<<" operator to print the values related to this tent
  cout << endl << myTent << endl;
}


