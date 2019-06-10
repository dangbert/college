// Do not change any of these 

#include <stdlib.h>
#include <iostream>
using namespace std ;

#include "linked.h"

LinkedList::LinkedList() {
   m_header = new Node() ;
   m_size = 0 ;
}


LinkedList::~LinkedList() {
   Node *current, *next ;

   // cout << "LinkedList destructor called\n" ;
   current = m_header ;

   // Run down the linked list and free everyone!

   while (current != NULL) {
      next = current->m_next ;
      current->m_next = NULL ;
      delete current ;
      current = next ;
   }
}


// adds just a Character
void LinkedList::add(Character *cptr) {
   Node *newNode = new Node() ;
   newNode->m_cptr = cptr ;
   newNode->m_next = m_header->m_next ;
   m_header->m_next = newNode ;
   m_size++ ;
}


// add a whole node
void LinkedList::addNode(Node *ptr) {
   ptr->m_next = m_header->m_next ;
   m_header->m_next = ptr ;
   m_size++ ;
}


// remove a node from the front.
Node *LinkedList::popNode() {
   Node *ptr ;

   if (m_header->m_next == NULL) return NULL ;

   ptr = m_header->m_next ;
   m_header->m_next = ptr->m_next ;
   ptr->m_next = NULL ;
   m_size-- ;

   return ptr ;
}


// Shallow copy of B to the host.
// Then B is eliminated.
void LinkedList::slurp(LinkedList& B) {
   Node *current, *next ;

   // delete old list
   current = m_header->m_next ;
   while (current != NULL) {
      next = current->m_next ;
      current->m_next = NULL ;
      delete current ;
      current = next ;
   }

   // take B's data
   m_header->m_next = B.m_header->m_next ;
   m_size = B.m_size ;

   // clear B
   B.m_header->m_next = NULL ;
   B.m_size = 0 ;
}

   
unsigned int LinkedList::size() {
   return m_size ;
}


Node *LinkedList::at(unsigned int i) {
   Node *current ;

   current = m_header ;
   while(1) {
      if (current->m_next == NULL) {
         cerr << "Error: LinkedList::at() bad index!\n" ;
         return NULL ;  // not found  
      }
      current = current->m_next ;
      if (i == 0) return current ;
      i-- ;
   }
}


Node *LinkedList::remove(Character *cptr) {
   Node *current, *prev ;

   current = m_header ;
   while(1) {
      if (current->m_next == NULL) {
         return NULL ;  // not found  
      }

      prev = current ;
      current = current->m_next ;

      if (current->m_cptr == cptr) {
         prev->m_next = current->m_next ;
         current->m_next = NULL ;
         m_size-- ;
         return current ;
      }
   }
}
