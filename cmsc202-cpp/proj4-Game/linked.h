#ifndef _LINKED_H_
#define _LINKED_H_

class Character ;
class Game; 

#include "node.h"

class LinkedList {
   friend class Game ;

public:
   LinkedList() ;
   ~LinkedList() ;
   void add(Character *cptr) ;
   void addNode(Node *ptr) ;
   Node *popNode() ;
   void slurp(LinkedList& B) ;
   Node *remove(Character *cptr) ;
   unsigned int size() ;

   Node *at(unsigned int i) ;

private:
   unsigned int m_size ;
   Node *m_header ;
} ;


#endif
