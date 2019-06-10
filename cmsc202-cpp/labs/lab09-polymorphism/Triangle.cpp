#include "Triangle.h"
#include "Shape.h"
#include <iostream>

using namespace std;

Triangle::Triangle(int length, int height) {
  m_length = length;
  m_height = height;
}

Triangle::~Triangle() {

}


int Triangle::GetArea() const {
  return 1/2 * m_length * m_height;
}


void Triangle::Draw() const {
  cout << "A triangle was drawn!" << endl;
}
