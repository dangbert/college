#ifndef _LLAMANODE_CPP_
#define _LLAMANODE_CPP_


/* File: LlamaNode.cpp

   UMBC CMSC 341 Fall 2016 Project 1

   This file has the implementations of the LlamaNode class
   for Project 1. See project description for details.

   This file should not be modified in ANY way.

   When your program is graded, it will be compiled with the original 
   version of this file. Your program must work with the original. 

*/


#include <iostream>
#include "LlamaNode.h"
using namespace std ;


// Static variables

template <class T, int LN_SIZE>
int LlamaNode<T,LN_SIZE>::newCount = 0 ;


template <class T, int LN_SIZE>
int LlamaNode<T,LN_SIZE>::deleteCount = 0 ;


template <class T, int LN_SIZE>
LlamaNode<T,LN_SIZE>::LlamaNode() {
   m_next = NULL ; 
   newCount++ ; 
}


template <class T, int LN_SIZE>
LlamaNode<T,LN_SIZE>::~LlamaNode() { 
   m_next = NULL ; 
   deleteCount++ ; 
}


template <class T, int LN_SIZE>
void LlamaNode<T,LN_SIZE>::report() {

   cerr << "# of nodes created = " << LlamaNode<T,LN_SIZE>::newCount << endl ;
   cerr << "# of nodes destroyed = " << LlamaNode<T,LN_SIZE>::deleteCount << endl ;

}


#endif
