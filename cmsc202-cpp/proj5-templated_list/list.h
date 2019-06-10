#ifndef LIST_H
#define LIST_H

#include "node.h"

template <class T> class LinkedList {
 public:
  LinkedList();
  ~LinkedList();
  LinkedList(const LinkedList<T>& otherList); //copy constructor
  void addFront(const T& moreData);
  T deleteFront();
  bool remove(const T& searchData);
  unsigned int size() const;
  T& operator[](unsigned int i);
  const LinkedList<T>& operator=(const LinkedList<T>& otherList);
  void print();

 private:
  Node<T> *m_head;     //dummy node for list head
  unsigned int m_size; //size of the list
  Node<T> *m_cache;    //cached node
  int m_cacheLoc;      //location of the cached node in array. -1 = invalid
};

#include "list.cpp"
#endif
