#include "Apartment.h"

using namespace std;

Apartment::Apartment(int st, bool wash) : Residence(4, 4, wash){
  story = st;
  washer = wash;
  //cout << "Apartment Constructor: getNumRooms()" << Residence::GetNumRooms() << endl;
}


double Apartment::PropertyValue() const{

  //  cout << "printing wash * 100: " << washer*100;

  return Residence::GetNumRooms() * 10000 + (int) (washer * 100);
}


int Apartment::GetStory() const{
  return story;
}

ostream& operator<<(ostream &sout, const Apartment& a){
  //sout << "IN APPARTMENT OPERATOR:::" << endl;
  
  sout << "Number of Rooms: " << a.GetNumRooms() << endl;
  
  sout << "Number of Walls: " << a.GetNumWalls() << endl;
  sout << "Washer: ";
  if(a.washer)
    sout << "true" << endl;
  else
    sout << "false" << endl;
  sout << "Number of windows: " << a.NumWindows() << endl;
  sout << "Property value: " << a.PropertyValue() << endl;
  sout << "Story: " << a.GetStory() << endl;
}
