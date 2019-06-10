#include<iostream>
#include "Shape.h"
#include "Rectangle.h"
#include "Triangle.h"

using namespace std;

int main()
{

  Shape *shapePtr;	       // Base class pointer 
  Rectangle aRectangle(5,4);  // creating a rectangle object
  Triangle aTriangle(4,4);    // creating a triangle object

  // Write code to perform dynamic binding here
  shapePtr = &aRectangle;
  shapePtr->Draw();
  cout << "Rectangle area = " << shapePtr->GetArea() << endl;

  shapePtr = &aTriangle;
  shapePtr->Draw();
  cout << "Triangle area = " << shapePtr->GetArea() << endl;
  return 0;
}

