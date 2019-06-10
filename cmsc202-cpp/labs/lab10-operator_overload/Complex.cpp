#include "Complex.h"

// Complex class constructor
Complex::Complex(int real, int imaginary):m_real(real),m_imaginary(imaginary)
{
 
}

// Accessor that returns real part of complex number
int Complex::GetReal() const
{
  return m_real;
}

// Accessor that returns imaginary part of complex number
int Complex::GetImaginary() const
{
  return m_imaginary;
}

// Start writing your code here ....


//adds the host and the object passed to it. Returns a new object that is the sum
const Complex Complex::operator+(const Complex &rhs) const {
  Complex c3(m_real + rhs.GetReal(), m_imaginary + rhs.GetImaginary());
  return c3;
}


//negates the host and returns a new object
const Complex Complex::operator-() const {
  Complex c3(-1 * m_real, -1 * m_imaginary);
    return c3;
}


//non membetr function
ostream& operator<<(ostream& out, const Complex& number) {
  if( number.GetImaginary() > 0)
    out << number.GetReal() << " + " << number.GetImaginary() << "i" << endl;
  else
    out << number.GetReal() << " - " << -1 * number.GetImaginary() << "i" << endl;
}
