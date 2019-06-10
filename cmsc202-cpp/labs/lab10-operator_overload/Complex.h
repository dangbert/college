#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>

using namespace std;

class Complex
{
   public:
      // Constructor with default arguments
      Complex(int real = 1, int imaginary = 1);
      
      // Accessor method which return the real part 
      int GetReal() const;
      
      // Accessor method which return the imaginary part
      int GetImaginary() const;
      
      // Overloaded + operator as member function
      const Complex operator+ (const Complex& rhs) const;

      // Overloaded unary minus operator as member function 
      const Complex operator-() const;
    
   private:
      int m_real;
      int m_imaginary;
};


// Overloaded << operator as non-member function 
ostream & operator << (ostream& out, const Complex& number);

#endif
