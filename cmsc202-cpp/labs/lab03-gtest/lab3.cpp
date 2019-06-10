#include <iostream>
#include "lab3.h"
using namespace std;

#include "lab3.h"

int main() {

  /* Compute and display Factorial(i) for i = 5..10 */

  for (int i = 5; i <= 10; ++i)
    cout << "Factorial(" << i << ") = " << Factorial(i) << endl;

  /* Compute and display ChooseTwo(i) for i = 5..10 */

  for (int i = 5; i <= 10; ++i)
    cout << "ChooseTwo(" << i << ") = " << ChooseTwo(i) << endl;
}

int ChooseTwo(int n) {
  if (n < 2 || n > 32768)
    return -1;
  else
    return (n*(n-1))/2;
}

/* IMPLEMENT Factorial() HERE */
int Factorial(int n)
{
  if(n==0)
    return 1;
  if(n<0 || n>12)
    return -1;
  
  for(int i=n-1; i>0; i--)
    {
      n = n * i;
    }

  return n;
}
    
  
