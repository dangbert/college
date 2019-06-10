#ifndef TENT_H
#define TENT_H

using namespace std;
#include <iostream>
#include "Residence.h"

class Tent : public Residence{

 public:
  Tent();
  // friend ostream &operator<<(ostream &output, const Residence &r);                 


  double PropertyValue() const; //override the parent function
  int NumWindows() const;
  friend ostream& operator<<(ostream& sout, const Tent& t);
                                                                                    
};


#endif
