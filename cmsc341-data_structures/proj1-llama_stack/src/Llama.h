#ifndef _LLAMA_H_
#define _LLAMA_H_

/* File: Llama.h

   UMBC CMSC 341 Fall 2016 Project 1

   This file has the class declaration for the LlamaNode class
   for Project 1. See project description for details.

   You may add public and private data members to the Llama class.

   You may add public and private member functions to the Llama class.

*/


#include <stdexcept>
#include <string>
#include "LlamaNode.h"

using namespace std ;


class LlamaUnderflow : public std::out_of_range {

   public:

   LlamaUnderflow(const string& what) : std::out_of_range(what) { }

} ;


template <class T, int LN_SIZE>
class Llama {

   public:

   Llama() ;
   Llama(const Llama<T,LN_SIZE>& other) ;   // copy constructor
   ~Llama() ;


   int size() ; 
   void dump() ;
   void push(const T& data) ;
   T pop() ;


   void dup() ;    //  (top) A B C D -> A A B C D
   void swap() ;   //  (top) A B C D -> B A C D 
   void rot() ;    //  (top) A B C D -> C A B D


   T peek(int offset) const ;


   // overloaded assignment operator
   //
   const Llama<T,LN_SIZE>& operator=(const Llama<T,LN_SIZE>& rhs) ;


   //
   // Add your public member functions & public data mebers here:
   //


   private:


   //
   // Add your private member functions & private data mebers here:
   //
   int getNextIndex() const;
   int getHeadCount() const;
   int getSize() const; // const version of size()
   int getNumNodes() const;
   const Llama<T,LN_SIZE>& copy(const Llama<T,LN_SIZE>& rhs); // make this a copy of other
   int m_size; // number of items in the llama stack

   LlamaNode<T,LN_SIZE> *m_head; // pointer to the the top node of the stack
   LlamaNode<T,LN_SIZE> *m_extra; // pointer to extra node before m_head (if it exists)


} ;


#include "Llama.cpp"


#endif
