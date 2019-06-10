#ifndef LIST_CPP
#define LIST_CPP

#include <iostream>
#include <stdlib.h>
#include <stdexcept> //for throwing a range_error

#include "list.h"

using namespace std;

template <class T>
LinkedList<T>::LinkedList()
  : m_head(new Node<T>())
{
  m_size = 0;
  m_cacheLoc = -1;
}


template <class T>
LinkedList<T>::~LinkedList() {
  Node<T> *current = m_head, *next;

  while(current != NULL) {
    next = current->m_next;
    delete current;
    current = next;
  }
}


//copy constructor
template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &otherList) {
  m_head = new Node<T>();
  m_size = 0;
  m_cacheLoc = -1;

  if(otherList.size() == 0)
    return;
  
  Node<T> *currentNew = m_head; //node pointer for the host object
  Node<T> *currentOther = otherList.m_head->m_next; //start at the first node in the list (after the dummy)
  
  for(int i=0; i<otherList.size(); i++) {
    currentNew->m_next = new Node<T>(currentOther->m_data); //create a deep copy of the current node in otherList
    m_size++; //keep track of the host's size
    currentNew = currentNew->m_next; //advance to the node that was just added
    currentOther = currentOther->m_next;
    }
}


//adds a new node to the beginning of the linked list with the given data
template <class T>
void LinkedList<T>::addFront(const T& moreData) {
  m_cacheLoc = -1; //indicate that the cache is no longer valid
  Node<T> *temp = m_head->m_next;
  m_head->m_next = new Node<T>(moreData); //create a new node at the front of the list
  m_head->m_next->m_next = temp; //link the new node to the rest of the list

  m_size++; //adjust the stored size of the list
}


//removes the first node of the linked list and returns the value stored in that node.
template <class T>
T LinkedList<T>::deleteFront() {
  m_cacheLoc = -1; //indicate that the cache is no longer valid
  if(size() == 0)  //throw a range error exception when the list is empty
    throw range_error("deleteFront() on empty list");

  Node<T> *temp = m_head->m_next->m_next; //store the second node in list
  int data = m_head->m_next->m_data; //store the data in the node to be deleted

  delete m_head->m_next;
  m_head->m_next = temp;
  m_size--;
  return data;
}


/*looks for a node with a given value and removes it from the linked list, if found. If there is more than one
node with the given value only the first occurrence is removed. If there is no node with value equal to searchData then calling remove should have no effect. The return value indicates whether the value is found*/
template <class T>
bool LinkedList<T>::remove(const T& searchData) {
  Node<T> *prev; //holds the node before current
  Node<T> *current = m_head->m_next;
  m_cacheLoc = -1; //indicate that the cache is no longer valid

  while(current != NULL && current->m_data != searchData) {
    prev = current;
    current = current->m_next;
  }

  if(current == NULL) //if the end of the list is reached without finding the searchData
    return false;

  Node<T> *next = current->m_next; //holds the first node after the node that will be reomved
  delete current;
  m_size--; //update the list size

  prev->m_next = next;
  return true;
}


//returns the number of nodes in the linked list (not counting the dummy header)
template <class T>
unsigned int LinkedList<T>::size() const {
  return m_size;
}


/*Returns a reference to the data in the i-th node of the linked list. This function must implement the caching strategy described above. The function returns a reference so we can assign values to the i-th node of the linked list.  Each time the operator[] function is invoked, it should cache a new pointer and index.
*/
template <class T>
T& LinkedList<T>::operator[](unsigned int i) {
  if(i >= size())
    throw range_error("invalid index in operator[]");

  Node<T> *current = m_head->m_next;

  if(m_cacheLoc != -1 && m_cacheLoc <= i) { //when the cache can be used
    current = m_cache;
    for(int p=0; p<i-m_cacheLoc; p++) //advance until reaching the ith node in the list
      current = current->m_next;
  }
  else { //when the cache can't be used, start at the begininning of the list
    for(int p=0; p<i; p++) //advance until reaching the ith node in the list
      current = current->m_next;
  }

  m_cacheLoc = i;
  m_cache = current;
  return current->m_data;
}


/*This should follow the usual guidelines for implementing assignment and also make a deep copy of the linked list. Note that the new list will not have a valid cache even if otherList has a valid cache.*/
template <class T>
const LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &otherList) {
  m_cacheLoc = -1; //indicate that the cache is now invalid

  while(size() != 0) //delete all nodes
    deleteFront();

  if(otherList.size() == 0) //return the host (now with only the dummy header) if the otherList is empty
    return *this;
  
  Node<T> *currentNew = m_head; //node pointer for the host object
  Node<T> *currentOther = otherList.m_head->m_next; //start at the first node in the list (after the dummy)
  
  for(int i=0; i<otherList.size(); i++) {
    currentNew->m_next = new Node<T>(currentOther->m_data); //create a deep copy of the current node in otherList
    m_size++; //keep track of the hosts size
    currentNew = currentNew->m_next; //advance to the node that was just added
    currentOther = currentOther->m_next;
    }  
  return *this;
}


/*prints out the data in each node in the linked list, one item per line. You may assume that objects of the templated type work with the insertion operator << */
template <class T>
void LinkedList<T>::print() {
  Node<T> *current = m_head->m_next;
  //I choose to do this because i think it's more efficient than using a for loop with operator[](i)
  while(current != NULL) {
    cout << current->m_data << endl;
    current = current->m_next;
    }
}

#endif
