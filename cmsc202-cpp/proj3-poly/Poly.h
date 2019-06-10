#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include "Node.h"

using namespace std;

class Polynomial {

 public:

  // Do not change the member function prototypes for
  // any public member function.

  // See documentation in Project 3 description.

  Polynomial();
  Polynomial(const Polynomial& p);
  ~Polynomial();

  void insertMonomial(long coeff, unsigned int deg);

  Polynomial add(const Polynomial& p) const;
  Polynomial subtract(const Polynomial& p) const;
  Polynomial multiply(const Polynomial& p) const;

  Polynomial modulo(const Polynomial& p) const;  // Extra Credit

  unsigned int degree() const;
  long evaluate(long x) const;

  Polynomial& operator=(const Polynomial& p);
  friend ostream& operator<<(ostream& sout, const Polynomial& p);
  void addTo(long coeff, unsigned int deg); //temp, make private later
 private:

  // Do not change the declaration of m_head

  Node *m_head; //pointer to the first node in the linked list

  // Declarations for Additional private member functions:

  //precondition: coeff and deg are the ceofficient and degree (respectively) of a monomial to be added
  //post condition: the host polynomial has a new monomial mathematically added to it (not just inserted)

};

#endif
