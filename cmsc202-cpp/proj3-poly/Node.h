#ifndef NODE_H
#define NODE_H

using namespace std;

// Do not change any part of this file

// See documentation in Project 3 description.

class Node {

 public:

  Node();
  Node(long coeff, unsigned int deg);
  
  long m_coefficient; //stores the coefficient of a monomial
  unsigned int m_degree; //stores the degree (power of x) of a monomial
  Node *m_next; //pointer to the next node in the linked list
};

#endif
