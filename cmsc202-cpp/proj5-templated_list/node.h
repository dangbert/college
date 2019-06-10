#ifndef NODE_H
#define NODE_H

template <class T> class Node {
 public:
  Node(); /*default contructor; used in the contructors of LinkedList 
	    needed for vectors to work because you can't initiate m_head with the int 0 when T is a vector*/ 
  Node(T data); //constructor for initializing a node with data
  T m_data;
  Node *m_next;
};

#include "node.cpp" //must be at bottom of file
#endif
