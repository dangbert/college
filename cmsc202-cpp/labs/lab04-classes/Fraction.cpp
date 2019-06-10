#include <iostream>
#include "Fraction.h"
using namespace std;


Fraction::Fraction(int n, int d) //constructor
{
  num = n;
  den = d;
}

  void Fraction::Output()
{
  cout << num << "/" << den << endl; 

}

void Fraction::Reciprocal()
{
  int temp = num;
  num = den;
  den = temp;
}
