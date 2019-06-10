#include <iostream>
using namespace std ;

#include "node.h"
#include "character.h"

Node::Node() {
   m_next = NULL ;
   m_cptr = NULL ;
}


Node::~Node() {
   // cout << "Node destructor called.\n" ;
   m_next = NULL ;
   delete m_cptr ;
   m_cptr = NULL ;
}
