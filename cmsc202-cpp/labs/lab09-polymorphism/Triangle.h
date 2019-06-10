#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Shape.h"

class Triangle : public Shape
{
 public:
//if the constructor is called without parameters, 0 will be used by default
  Triangle(int length=0, int height=0); 
  virtual ~Triangle();
  virtual int GetArea() const;
  virtual void Draw() const;

 private:
  int m_length;
  int m_height;
};

#endif
