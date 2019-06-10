#ifndef APARTMENT_H
#define APARTMENT_H

using namespace std;
#include <iostream>
#include "Residence.h"


class Apartment : public Residence {

 public:
  Apartment(int st, bool wash); //story, washer
  // friend ostream &operator<<(ostream &output, const Residence &r);                  
  double PropertyValue() const;
  int GetStory() const;
  friend ostream& operator<<(ostream& sout, const Apartment& a);

  private:
  bool washer;
  int story;


};


#endif
