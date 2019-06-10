#ifndef MAX_HEAP_CPP_
#define MAX_HEAP_CPP_

#include "MaxHeap.h"
#include <assert.h>
#include <iostream>
using std::cout;
using std::endl;


/*
 * default constructor
 * calls Heap constructor
 */
template<class T, int m_size>
MaxHeap<T, m_size>::MaxHeap()
    : Heap<T, m_size>()
{

}

template<class T, int m_size>
//MaxHeap<T, m_size>::MaxHeap(const MaxHeap<T, m_size>& origHeap)
MaxHeap<T, m_size>::MaxHeap(const Heap<T, m_size>& origHeap)
    : Heap<T, m_size>(origHeap)
{

}

/*
 * returns true if the objects at the given indexes need to be swapped
 * returns false otherwise
 */
template<class T, int m_size>
bool MaxHeap<T, m_size>::NeedSwap(int parentIndex, int childIndex) {
    if (this->m_array[parentIndex].GetValue() < this->m_array[childIndex].GetValue())
        return true;
    return false;
}

/*
 * compares the values stored in the objects at the given indexes in m_array
 * returns the index corrsponding to a greater value
 * returns the left index if both have the same value
 */
template<class T, int m_size>
int MaxHeap<T, m_size>::Compare(int leftIndex, int rightIndex) {
    int childIndex = leftIndex;
    if (rightIndex != -1) {
        if (this->m_array[rightIndex].GetValue() > this->m_array[leftIndex].GetValue())
            childIndex = rightIndex;
    }
    return childIndex;
}

#endif
