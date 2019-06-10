#include <iostream>
#include "Complex.h"
using namespace std;

int main() {

  // To test your code, you must
  // - create at least three Complex numbers using
  //   the non-default constructor.
  Complex com1(1.0, 4.0);
  Complex com2 (2.5, 6.5);
  Complex com3(2.0, 1.0);

  // - Use the Multiply() function to compute the                                     
  //   product of two Complex numbers; output the                                     
  //   product using the ToString() function.  
  cout << com1.Multiply(com3).ToString() << endl;

  // - Use the Multiply function to compute the                                       
  //   product of a Complex number and a double;                                      
  //   output the product using the ToString()                                        
  //   function. 
  cout << com2.Multiply(2.0).ToString() << endl;

  return 0;
}
  
