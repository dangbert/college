#ifndef COMPLEX_H
#define COMPLEX_H

#include <string>
using namespace std;

class Complex {
 public:

  // Complex() - default constructor
  // Creates a complex number with real part 1.0, imaginary part 0.0
  Complex();

  // Complex() - non-default constructor
  // Creates a complex number with real part re, imaginary part im
  Complex(double re, double im);

  // GetReal() - returns real part of complex number
  double GetReal() const;

  // GetImag() - returns imaginary part of complex number
  double GetImag() const;

  // Add() - adds two complex numbers
  Complex Add(Complex a) const;

  //
  // Multiply() DECLARATIONS GO HERE
  Complex Multiply(Complex a) const;
  Complex Multiply(double r) const;

  // ToString() - returns a string representation of a complex number
  string ToString() const;

 private:
  double m_real;  // real part
  double m_imag;  // imaginary part
};
  
#endif
