#ifndef MIN_HEAP_CPP_
#define MIN_HEAP_CPP_

#include "MinHeap.h"
#include <assert.h>
#include <iostream>
using std::cout;
using std::endl;


/*
 * default constructor
 * calls Heap constructor
 */
template<class T, int m_size>
MinHeap<T, m_size>::MinHeap()
    : Heap<T, m_size>()
{

}

template<class T, int m_size>
//MinHeap<T, m_size>::MinHeap(const MinHeap<T, m_size>& origHeap)
MinHeap<T, m_size>::MinHeap(const Heap<T, m_size>& origHeap)
    : Heap<T, m_size>(origHeap)
{

}

/*
 * returns true if the objects at the given indexes need to be swapped
 * returns false otherwise
 */
template<class T, int m_size>
bool MinHeap<T, m_size>::NeedSwap(int parentIndex, int childIndex) {
    if (this->m_array[parentIndex].GetValue() > this->m_array[childIndex].GetValue())
        return true;
    return false;
}

/*
 * compares the values stored in the objects at the given indexes in m_array
 * returns the index corrsponding to a smaller value
 * returns the left index if both have the same value
 */
template<class T, int m_size>
int MinHeap<T, m_size>::Compare(int leftIndex, int rightIndex) {
    int childIndex = leftIndex;
    if (rightIndex != -1) {
        if (this->m_array[rightIndex].GetValue() < this->m_array[leftIndex].GetValue())
            childIndex = rightIndex;
    }
    return childIndex;
}

#endif
