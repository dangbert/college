#include "Node.h"
#include <iostream>

using namespace std;


//Default constructor, initializes m_next to NULL.
Node::Node(){
  m_next = NULL;
}


//Initializes m_ceofficient to coeff, m_degree to deg, and m_next to NULL.
Node::Node(long coeff, unsigned int deg){
  m_coefficient = coeff;
  m_degree = deg;
  m_next = NULL;
}



/*
long m_coefficient-    stores the coefficient of a monomial
unsigned int m_degree- stores the degree (power of x) of a monomial
Node *m_next-          pointer to the next node in the linked list
*/
