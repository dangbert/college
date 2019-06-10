#ifndef NODE_CPP
#define NODE_CPP

#include "node.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

template <class T>
Node<T>::Node() {
  m_next = NULL;
}

template <class T>
Node<T>::Node(T data)
  : m_data(data) 
{
  m_next = NULL;
}

#endif
