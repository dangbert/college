#include <iostream>
#include "Polynomial.h"
#include <ctime>

using namespace std;

int main() {

  cout << endl << "*** Tests of modulo()" << endl << endl;

  // Construct p(x) = 13 x^5 + 7 x^2 + 1

  Polynomial p;
  p.insertMonomial(13, 0);
  p.insertMonomial(7, 2);
  p.insertMonomial(1, 5);

  cout << "p(x) = " << p << endl;

  // Construct q(x) = x^3 + 7 x + 3

  Polynomial q;
  q.insertMonomial(1, 3);
  q.insertMonomial(7, 1);
  q.insertMonomial(3, 0);
  
  cout << "q(x) = " << q << endl << endl;

  Polynomial mod = p.modulo(q);
  cout << "  p(x) mod q(x) = " << mod << endl;
  mod = q.modulo(p);
  cout << "  q(x) mod p(x) = " << mod << endl << endl;

  // Construct r(x) = x^10 + 2 x^9 + 3 x^8

  Polynomial r;
  r.insertMonomial(1,10);
  r.insertMonomial(2,9);
  r.insertMonomial(3,8);

  cout << "r(x) = " << r << endl;

  // Construct s(x) = x^6 - 2 x^4 + 4 x^2 - 8

  Polynomial s; 
  s.insertMonomial(1, 6);
  s.insertMonomial(-2, 4);
  s.insertMonomial(4, 2);
  s.insertMonomial(-8, 0);

  cout << "s(x) = " << s << endl << endl;
  
  mod = r.modulo(s);
  cout << "  r(x) mod s(x) = " << mod << endl;
  mod = s.modulo(r);
  cout << "  s(x) mod r(x) = " << mod << endl << endl;


  // Construct t(x) = (x - 10)(x - 9)...(x - 1)

  Polynomial t;
  t.insertMonomial(1,0);
  for (int i = 1; i <= 10; i++) {
    Polynomial temp;
    temp.insertMonomial(1,1);
    temp.insertMonomial(-i,0);
    t = t.multiply(temp);
  }

  cout << "t(x) = " << t << endl << endl;

  // Compue t(x) mod (x - i) for i = 0, 1, ..., 10

  for (int i = 0; i <= 10; i++) {
    Polynomial div;
    div.insertMonomial(1, 1);
    div.insertMonomial(-i, 0);
    mod = t.modulo(div);
    cout << "  t(x) mod " << div << " = " << mod << endl;
  }
  
  cout << endl;

  return 0;
}
