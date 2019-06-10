#include "List.h"
#include <iostream>
#include <stdlib.h>

using namespace std;


Node::Node(int data)
  : m_data(data) 
{
  m_next = NULL;
}


List::List()
  : m_head(new Node(0)) //dummy 0 node
{ /* No code */ }


List::~List() {
  Node *current, *next;

  current = m_head;
  while (current != NULL) {
    next = current->m_next;	// Save this away while we still have "curr"
    delete current;
    current = next;		// Now, move on to next item
  }
}


void List::AddToEnd(int data) {
  Node* current = m_head;
    
  // traverse linked list to end
  while(current->m_next != NULL) {
    current = current->m_next;
  }

  // Now, "current" points to last Node on list--add new Node after that
  //  (Note: you must allocate a new Node object from the heap to hold
  //   the new data)
  // Before:   [CURRENT] -> NULL
  // After: [CURRENT] -> [NEW NODE] -> NULL

  // PUT CODE HERE:
 
  current -> m_next = new Node(data);
 //current->m_next is the same as (*current.m_next)

}

int List::RemoveFromStart() {
  // First, check that there is something in the list;
  // IMPORTANT: Don't forget the dummy 0 at the head of the list!

  if (m_head == NULL || m_head->m_next == NULL) {
    cout << "Trying to remove from an empty list--exiting!!\n";
    exit(1);
  }


  // Now, remove the first item on the list.  Note that m_head points
  // to a dummy "0" Node, so we don't remove that: we remove the one
  // AFTER that.
  // Don't forget that you need to delete the removed Node, but
  // also don't forget that you have to extract and save any needed
  // data members of the Node first, because it is illegal to access
  // anything inside an object once you delete it!!
  //
  // Before:  m_head -> [DUMMY 0 NODE] -> [FIRST_NODE] -> <REST OF NODES>
  // After:   m_head -> [DUMMY 0 NODE] -> <REST OF NODES>
  // Return: data from FIRST_NODE

  // PUT CODE HERE:

  //cout << "START: "; Print(); cout << endl;
  Node* temp = m_head->m_next; //pointer to the node that will be delted
  int n = temp->m_data;        //the data in the node to be deleted
  m_head->m_next=temp->m_next; //change m_head to point to the node after the one that will be deleted
  delete temp; //i'm not sure if this actually deltes the node or just the pointer

  //cout << "END: "; Print(); cout << endl;

  return n;  // Replace VALUE_TO_RETURN with actual value
}


void List::Print() const {
  Node* current = m_head->m_next;  // Skip over dummy 0 Node

  while(current != NULL) {
    cout << current->m_data << ' ';
    current = current->m_next;
  }
}


unsigned int List::Size() const {
  // traverse linked list with a while loop and count the number of
  // elements.  Don't forget about dummy 0 node at head of list!

  // PUT CODE HERE:
 
  Node* current = m_head;
 int size = 0; 
  while(current -> m_next != NULL)
    {
      current = current -> m_next;
      size++;  //add 1 because a node was found   
    }

  return size;  // Replace VALUE_TO_RETURN with actual value
}
