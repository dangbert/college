#ifndef RESIDENCE_H
#define RESIDENCE_H

using namespace std;
#include <iostream>


class Residence {
  
public:
  Residence(int nR, int nW, bool hW);

  double PropertyValue() const;

  int NumWindows() const;

  friend ostream &operator<<(ostream &sout, const Residence &r); 
  int GetNumRooms() const;
  int GetNumWalls() const;
  
 protected:
  bool HasWasher() const;

private:
  int numRooms;
  int numWalls; 
  bool hasWasher;

};


#endif


