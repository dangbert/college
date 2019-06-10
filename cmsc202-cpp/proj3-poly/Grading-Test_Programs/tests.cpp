#include <iostream>
#include <ctime>
#include "Polynomial.h"

using namespace std;

int main() {

  // p(x) = 13 x^5 + 7 x^2 + 1
  Polynomial p;
  p.insertMonomial(1, 0);
  p.insertMonomial(7, 2);
  p.insertMonomial(13, 5);

  // q(x) = 2 x^7 - 5 x^2 + 1
  Polynomial q;
  q.insertMonomial(1, 0);
  q.insertMonomial(-5, 2);
  q.insertMonomial(2, 7);

  // r(x) = (x - 2)(x - 1) = x^2 - 3 x + 2
  Polynomial r;
  r.insertMonomial(1, 2);
  r.insertMonomial(-3, 1);
  r.insertMonomial(2, 0);

  // a(x) = x^17 + 141 x^12 + 3 x^10
  Polynomial a;
  a.insertMonomial(1, 17);
  a.insertMonomial(141, 12);
  a.insertMonomial(3, 10);

  // three(x) = 3
  Polynomial three;
  three.insertMonomial(3, 0);

  // zero(x) = 0
  Polynomial zero;

  //
  // EVALUATE AND DEGREE TESTS
  //

  cout << endl << "*** Tests of evaluate() and degree()" << endl << endl;

  cout << "p(x) = " << p << endl << endl;

  cout << "  p has degree " << p.degree() << endl;
  cout << "  p(0) = " << p.evaluate(0) << endl;
  cout << "  p(-3) = " << p.evaluate(-3) << endl;
  cout << "  p(2) = " << p.evaluate(2) << endl << endl;

  cout << "q(x) = " << q << endl << endl;

  cout << "  q has degree " << q.degree() << endl;
  cout << "  q(0) = " << q.evaluate(0) << endl;
  cout << "  p(-3) = " << q.evaluate(-3) << endl;
  cout << "  p(0) = " << q.evaluate(0) << endl << endl;

  cout << "r(x) = " << r << endl << endl;

  cout << "  r has degree " << r.degree() << endl;
  cout << "  r(0) = " << r.evaluate(0) << endl;
  cout << "  r(1) = " << r.evaluate(1) << endl;
  cout << "  r(2) = " << r.evaluate(2) << endl << endl;

  cout << "zero(x) = " << zero << endl << endl;

  cout << "  zero(1) = " << zero.evaluate(1) << " (should be 0)" << endl;
  cout << "  Degree of zero() is " << zero.degree() << endl;
  cout << "  Degree of zero polynomial is technically -infinity;" << endl
       << "  Should return something sensible, e.g. zero or negative." << endl << endl;

  cout << "three(x) = " << three << endl << endl;
  
  cout << "  three(0) = " << three.evaluate(0) << " (should be 3)" << endl;
  cout << "  Degree of constant 3 is " << three.degree() << endl 
       << "  Degree of a non-zero constant should be zero." << endl << endl;

  //
  // ADDITION AND SUBTRACTION TESTS
  //
    cout << "*** Tests of add() and subtract()" << endl << endl;

  cout << "p(x) = " << p << endl << endl;

  cout << "q(x) = " << q << endl << endl;

  cout << "a(x) = " << a << endl << endl;

  Polynomial sum = p.add(q);
  cout << "  p(x) + q(x) = " << sum << endl;
  cout << "  The sum has degree " << sum.degree() << endl << endl;

  sum = q.add(p);
  cout << "  q(x) + p(x) = " << sum << endl;
  cout << "  The sum has degree " << sum.degree() << endl << endl;

  sum = a.add(p);
  cout << "  a(x) + p(x) = " << sum << endl;
  cout << "  The sum has degree " << sum.degree() << endl << endl;

  Polynomial dif = p.subtract(q);
  cout << "  p(x) - q(x) = " << dif << endl;
  cout << "  The difference has degree " << dif.degree() << endl << endl;

  dif = q.subtract(p);
  cout << "  q(x) - p(x) = " << dif << endl;
  cout << "  The difference has degree " << dif.degree() << endl << endl;

  sum = p.add(zero);
  cout << "  p(x) + 0 = " << sum << endl;
  sum = zero.add(p);
  cout << "  0 + p(x) = " << sum << endl;
  dif = p.subtract(zero);
  cout << "  p(x) - 0 = " << dif << endl;
  dif = zero.subtract(p);
  cout << "  0 - p(x) = " << dif << endl << endl;

  cout << "Creating two degree 10000, dense polynomials c and d..." << endl;
  Polynomial c, d;
  for (int i = 1; i <= 1001; i++) {
    c.insertMonomial(i, i-1);
    d.insertMonomial(i+10, i-1);
  }
  cout << "  Degree of c(x) is " << c.degree()
       << ", degree of d(x) is " << d.degree() << endl;
  cout << "  Adding c to d.  Should complete in under a second...";
  clock_t t0 = clock();
  sum = c.add(d);
  clock_t t = clock() - t0;
  cout << "done (" << t / ((double) CLOCKS_PER_SEC) << " sec)" << endl;

  cout << "  Subtracting d from c.  Should complete in under a second...";
  t0 = clock();
  sum = c.subtract(d);
  t = clock() - t0;
  cout << "done (" << t / ((double) CLOCKS_PER_SEC) << " sec)" << endl << endl;

  
  //
  // MULTIPLICATION TESTS
  //

  cout << "*** Tests of multiply()" << endl << endl;

  cout << "p(x) = " << p << endl << endl;

  cout << "q(x) = " << q << endl << endl;  

  cout << "a(x) = " << a << endl << endl;

  Polynomial prod = p.multiply(q);
  cout << "  p(x)q(x) = " << prod << endl;
  cout << "  The product has degree " << prod.degree() << endl << endl;

  prod = q.multiply(p);
  cout << "  q(x)p(x) = " << prod << endl;
  cout << "  The product has degree " << prod.degree() << endl << endl;

  prod = a.multiply(p);
  cout << "  a(x)p(x) = " << prod << endl;
  prod = a.multiply(q);
  cout << "  a(x)q(x) = " << prod << endl << endl;

  prod = three.multiply(p);
  cout << "  3*p(x) = " << prod << endl << endl;

  prod = zero.multiply(p);
  cout << "  0*p(x) = " << prod << endl << endl;

  cout << "Creating a degree 100, dense polynomial b1..." << endl;
  Polynomial b1;
  for (int i = 1; i <= 101 ; i++) 
    b1.insertMonomial(i, i-1);
  cout << "  Degree of b1(x) is " << b1.degree() << endl;
  cout << "  Squaring b1.  Should complete in under a second...";
  t0 = clock();
  prod = b1.multiply(b1);
  t = clock() - t0;
  cout << "done (" << t / ((double) CLOCKS_PER_SEC) << " sec)" << endl;
  cout << "  Degree of b1(x) * b1(x) is " << prod.degree() << endl << endl;
  
  cout << "Creating a degree 500, dense polynomial b2..." << endl;
  Polynomial b2;
  for (int i = 1; i <= 501 ; i++) 
    b2.insertMonomial(i, i-1);
  cout << "  Degree of b2(x) is " << b2.degree() << endl;
  cout << "  Squaring b2.  Should complete in a second or two...";
  t0 = clock();
  prod = b2.multiply(b2);
  t = clock() - t0;
  cout << "done (" << t / ((double) CLOCKS_PER_SEC) << " sec)" << endl;
  cout << "  Degree of b2(x) * b2(x) is " << prod.degree() << endl << endl;

  return 0;
}
