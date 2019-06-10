#ifndef _LLAMA_CPP_
#define _LLAMA_CPP_

/* File: Llama.cpp

   Daniel Engbert
   UMBC CMSC 341 Fall 2016 Project 1
   This file implements the Llama class (Linked-List Array Mixed Amalgamated Stack)

   This class is a stack that is made of a linked list of nodes, each containing
   an array.

   m_head always points to the node at the top of the stack
   extra nodes aren't removed until the previous node is half empty

*/

#include <iostream>
#include "Llama.h"
using namespace std;


template <class T, int LN_SIZE>
Llama<T, LN_SIZE>::Llama() {
    m_head = new LlamaNode<T,LN_SIZE>;
    m_size = 0;
    m_extra = NULL;
}

/*
 * copy constructor
 */
template <class T, int LN_SIZE>
Llama<T, LN_SIZE>::Llama(const Llama<T,LN_SIZE>& other) {
    m_head = new LlamaNode<T,LN_SIZE>;
    m_size = 0;
    m_extra = NULL;
    copy(other);
}

/*
 * overloaded assignment operator
 * handles the case of self-assignment
 */
template <class T, int LN_SIZE>
const Llama<T,LN_SIZE>& Llama<T, LN_SIZE>::operator=(const Llama<T,LN_SIZE>& rhs) {
    return copy(rhs);

}

template <class T, int LN_SIZE>
Llama<T, LN_SIZE>::~Llama() {
    LlamaNode<T, LN_SIZE> *current = m_head, *next;
    while (current != NULL) {
        next = current->m_next;
        delete current;
        current = next;
    }

    if (m_extra != NULL) {
        delete m_extra;
        m_extra = NULL;
    }
}

/*
 * makes this object a copy of rhs
 *
 * helper function for copy constructor and assignment operator:
 * adds or removes nodes until there's just enough to store the copied data
 * then uses the default LlamaNode assignment operator to set nodes equal.
 */
template <class T, int LN_SIZE>
const Llama<T,LN_SIZE>& Llama<T, LN_SIZE>::copy(const Llama<T,LN_SIZE>& rhs) {

    if (m_head == rhs.m_head) { // check for self assigment
        return *this;
    }

    int numNodes = getNumNodes();
    int nodesNeeded = rhs.getNumNodes();

    // add as many nodes as needed
    for (int i=0; i<(nodesNeeded-numNodes); i++) {
        if (m_extra != NULL) {
            // make m_extra the new head
            m_extra->m_next = m_head;
            m_head = m_extra;
            m_extra = NULL;
        }
        else {
            LlamaNode<T, LN_SIZE> *temp = new LlamaNode<T,LN_SIZE>;
            temp->m_next = m_head;
            m_head = temp; // update m_head
        }
    }

    // remove as many nodes as needed
    for (int i=0; i<(numNodes-nodesNeeded); i++) {
        // remove a node
        LlamaNode<T, LN_SIZE> *temp = m_head;
        m_head = m_head->m_next;
        delete temp;
    }

    // if rhs has an extra node, make sure this also does
    if (rhs.m_extra != NULL && m_extra == NULL) {
        m_extra = new LlamaNode<T,LN_SIZE>;
    }
    // if rhs has no extra node, make sure this also doesn't have one
    if (rhs.m_extra == NULL && m_extra != NULL) {
        delete m_extra;
        m_extra = NULL;
    }
    // now this and rhs have the same number of nodes

    LlamaNode<T, LN_SIZE> *rhs_current=rhs.m_head, *current=m_head, *next;

    // copy nodes using the default LlamaNode assignment operator
    // you could instead use the default copy constructor to construct a copied node
    // example: temp = new LlamaNode<T, LN_SIZE>(*rhs_current);
    while (rhs_current != NULL) {
        next = current->m_next;
        *current = *rhs_current; // set this node = to the node in rhs
        current->m_next = next; // don't let it point into rhs

        rhs_current = rhs_current->m_next;
        current = current->m_next;
    }

    m_size = rhs.getSize();
    return *this;
}

/*
 * returns the number of items in the llama stack
 */
template <class T, int LN_SIZE>
int Llama<T, LN_SIZE>::size() {
    return m_size;
}

/*
 * const version of size()
 */
template <class T, int LN_SIZE>
int Llama<T, LN_SIZE>::getSize() const {
    return m_size;
}

/*
 * returns the number of nodes in use
 * assumes m_size is correct
 */
template <class T, int LN_SIZE>
int Llama<T, LN_SIZE>::getNumNodes() const {
    if (getSize() == 0)
        return 1;

    int numNodes = getSize() / LN_SIZE;
    if (getSize() % LN_SIZE != 0)
        numNodes++;

    return numNodes;
}

/*
 * Returns the index of the next free spot in m_head->arr
 */
template <class T, int LN_SIZE>
int Llama<T, LN_SIZE>::getNextIndex() const {
    return m_size % LN_SIZE;
}

/*
 * Returns the number of elements in m_head->arr
 */
template <class T, int LN_SIZE>
int Llama<T, LN_SIZE>::getHeadCount() const {
    int headCount = getNextIndex();
    if (getNextIndex() == 0 && m_size > 0) { // if the next index would be a new node/array
        headCount = LN_SIZE;
    }
    return headCount;
}

template <class T, int LN_SIZE>
void Llama<T, LN_SIZE>::dump() {
    cerr << endl << "***** Llama Stack Dump ******" << endl;
    cerr << "LN_SIZE = " << LN_SIZE << endl;
    cerr << "# of items in the stack = " << m_size << endl;
    // number of nodes ever created (not just nodes from this Llama)
    cerr << "# of nodes created = " << m_head->newCount << endl;
    cerr << "# of nodes destroyed = " << m_head->deleteCount << endl;

    if (m_extra == NULL)
        cerr << endl << "This stack does not have an extra node." << endl;
    else
        cerr << endl << "This stack has an extra node: " << m_extra << "." << endl;

    cerr << endl << "Stack contents, top to bottom" << endl;

    // print m_head
    cerr << "----- " << m_head << " ----- " << endl;
    for (int i=getHeadCount()-1; i>=0; i--) {
        cerr << m_head->arr[i] << endl;
    }

    // print the the rest of the nodes (they have full arrays)
    LlamaNode<T, LN_SIZE> *current = m_head->m_next;
    while (current != NULL) {
        cerr << "----- " << current << " ----- " << endl;
        // loop through the array starting at the bottom and moving to the top
        for (int i=LN_SIZE-1; i>=0; i--) {
            cerr << current->arr[i] << endl;
        }
        current = current->m_next;
    }

    cerr << "----- bottom of stack ---- " << endl;
    cerr << endl << "*****************************" << endl;
}

/*
 * Adds (a copy of) data to the top of the stack
 */
template <class T, int LN_SIZE>
void Llama<T, LN_SIZE>::push(const T& data) {
    if (getHeadCount() == LN_SIZE) { // if head node is full
        if (m_extra == NULL) { // if m_extra doesn't exist
            LlamaNode<T, LN_SIZE> *temp = new LlamaNode<T,LN_SIZE>;

            temp->m_next = m_head;
            m_head = temp; // update m_head
        }
        else {
            // make m_extra the new head
            m_head = m_extra;
            m_extra = NULL;
        }
    }

    m_head->arr[getNextIndex()] = data; // add data to the array in the top of the stack
    m_size++;
}

/*
 * Removes and returns the item at the top of the stack
 * (Last in, first out)
 */
template <class T, int LN_SIZE>
T Llama<T, LN_SIZE>::pop() {
    if (m_size == 0)
        throw LlamaUnderflow("unable to pop: llama is empty!");

    T val = peek(0); // element being removed
    m_size--; // just decrease the size, the element can be overwritten later

    // case m_head->arr is now empty (and m_head is not the only node)
    if (getNextIndex() == 0 && m_size > 0) {
        if (m_extra != NULL) { // if m_extra already exists
            delete m_extra;
            m_extra = NULL; // important: clear the stored address
        }
        // make m_head the new m_extra
        m_extra = m_head;
        m_head=m_head->m_next;
    }

    // delete m_extra if it exists and m_head-> arr is at half capacity
    if (m_extra != NULL && getHeadCount() == LN_SIZE/2) {
        delete m_extra;
        m_extra = NULL;
    }

    return val;
}

/*
 * Duplicates the top of the stack
 * (top) A B C D -> A A B C D
 */
template <class T, int LN_SIZE>
void Llama<T, LN_SIZE>::dup() {
    if (m_size == 0) {
        throw LlamaUnderflow("unable to duplicate: llama is empty!");
    }
    push(peek(0));

}

/*
 * Exchanges the top two items in the stack
 * (top) A B C D -> B A C D
 */
template <class T, int LN_SIZE>
void Llama<T, LN_SIZE>::swap() {
    if (m_size < 2) {
        throw LlamaUnderflow("unable to swap: llama has less than 2 elements!");
    }

    T temp0 = pop(); // old top of stack
    T temp1 = pop(); // old 2nd item in stack

    push(temp0);
    push(temp1);
}

/*
 * Permutes the top three items in the stack
 * (top) A B C D -> C A B D
 */
template <class T, int LN_SIZE> void Llama<T, LN_SIZE>::rot() {
    if (m_size < 3) {
        throw LlamaUnderflow("unable to rotate: llama has less than 3 elements!");
    }

    T temp0 = pop(); // old top of stack
    T temp1 = pop(); // old 2nd item in stack
    T temp2 = pop(); // old 3rd item in stack

    push(temp1);
    push(temp0);
    push(temp2);
}

/*
 * Returns the value of the element in the stack that is <offset> away from the top
 * Peek(0) is the top of the stack, Peek(1) is the first item below the top
 */
template <class T, int LN_SIZE>
T Llama<T, LN_SIZE>::peek(int offset) const {
    // check if the offset is in range before traversing the list
    if (offset+1 > m_size) {
        throw LlamaUnderflow("peek out of range!");
    }

    int headCount = getHeadCount(); // number of elements in m_head->arr

    // if m_head is the target node
    if (offset <= headCount-1) {
        return m_head->arr[(headCount-1) - offset];
    }

    LlamaNode<T, LN_SIZE> *target = m_head;
    int nodeLoc = (offset-headCount) / LN_SIZE + 1; // order of the target node in the chain of nodes
    int arrOffset = (offset-headCount) - ((nodeLoc-1) * LN_SIZE); // offset of target value in the target array

    for (int i=0; i<nodeLoc; i++) { // advance to the target node
        target = target->m_next;
    }
    return target->arr[(LN_SIZE-1) - arrOffset];
}

#endif
