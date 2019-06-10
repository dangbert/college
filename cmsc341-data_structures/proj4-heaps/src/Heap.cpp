#ifndef HEAP_CPP_
#define HEAP_CPP_
#include "Heap.h"
#include <assert.h>
#include <iostream>
using std::cout;
using std::endl;

/*
 * default constuctor
 * makes size 1 bigger than m_size so Remove can return a reference to the
 * "removed" object
 */
template<class T, int m_size>
Heap<T, m_size>::Heap() {
    // m_array is a pointer to a dynamically allocated array
    m_array = new T[m_size + 1]; // create array of type T
    m_actualSize = 0;
}

/*
 * copy constructor
 */
template<class T, int m_size>
Heap<T, m_size>::Heap(const Heap<T, m_size>& origHeap) {
    m_array = new T[m_size + 1];
    m_actualSize = origHeap.m_actualSize;
    for (int i=0; i<m_actualSize; i++) { // loop through entire array
        m_array[i] = origHeap.m_array[i];
    }
    m_array[m_size] = origHeap.m_array[m_size]; // copy last item in array
}

/*
 * destructor
 */
template<class T, int m_size>
Heap<T, m_size>::~Heap() {
    delete [] m_array; // deallocate the array
}

/*
 * returns true if the needle is found in the heap
 */
template<class T, int m_size>
bool Heap<T, m_size>::Contains(const T& needle) const {
    return Find(needle) != NULL;
}

/*
 * returns a pointer of type T if the needle is in the Heap
 * (i.e. an object with the same Key exists)
 * returns NULL if not found
 * may take up to O(n)
 */
template<class T, int m_size>
const T* Heap<T, m_size>::Find(const T& needle) const {
    for (int i=0; i<m_actualSize; i++) {
        if (m_array[i] == needle)
            return &this->m_array[i]; // pointer to found object
    }
    return NULL; // not found
}

/*
 * removes and returns a min/max value T (by reference) from the heap.
 * rather than deleting the object, it is moved to the last element in the array
 * (which is a free spot) so that a reference to it can be returned.
 * in the process of removal the heap is updated to maintain oder
 * should run in O(log n)
 */
template<class T, int m_size>
T& Heap<T, m_size>::Remove() {
    assert(m_actualSize > 0);

    // store object being removed in the free spot at the end of m_array
    m_array[m_size] = m_array[0];

    m_array[0] = m_array[m_actualSize-1]; // copy last object in heap to the root
    m_actualSize--; // update the number of objects in heap
    PercolateDown(0); // fix the heap
    return m_array[m_size]; // return reference to object "removed"
}

/*
 * insert a new object into the heap
 * if needed, the object will percolate up
 * should run in O(n)
 */
template<class T, int m_size>
void Heap<T, m_size>::Insert(T& insertable) {
    assert(m_actualSize <= m_size-1); // need at least 1 free space

    m_array[m_actualSize] = insertable; // store object in next available spot
    m_actualSize++;
    PercolateUp(m_actualSize - 1); // fix heap
}

/*
 * used internally on insert
 * should run in O(log n)
 */
template<class T, int m_size>
void Heap<T, m_size>::PercolateUp(int index) {
    if (index == 0)
        return;
    int parentIndex = GetParentIndex(index);

    // flip parent and child positions if needed
    if (Swap(parentIndex, index))
        PercolateUp(parentIndex); // if swap was performed
}

/*
 * used internally on Remove
 * should run in O(log n)
 */
template<class T, int m_size>
void Heap<T, m_size>::PercolateDown(int index) {
    int leftIndex = GetLeftIndex(index); // left child index
    int rightIndex = GetRightIndex(index); // right child index
    if (leftIndex == -1) // if left (and right) children don't exist
        return;

    // get the index of the child to compare with
    int childIndex = Compare(leftIndex, rightIndex);

    // flip parent and child positions if needed
    if (Swap(index, childIndex))
        PercolateDown(childIndex); // if swap was performed
}

/*
 * swaps the objects at the given indexes in m_array if needed
 * returns true if a swap was needed, false otherwise
 */
template<class T, int m_size>
bool Heap<T, m_size>::Swap(int parentIndex, int childIndex) {
    if (NeedSwap(parentIndex, childIndex)) { // if swap is needed
        T temp = m_array[parentIndex];
        m_array[parentIndex] = m_array[childIndex];
        m_array[childIndex] = temp;
        PercolateUp(parentIndex);
        return true; // indicate that a swap was needed/performed
    }
    return false; // a swap wasn't needed
}

/*
 * returns the index of the left child node of the node at nodeIndex
 * returns -1 if no object is stored in the resulting index
 */
template<class T, int m_size>
int Heap<T, m_size>::GetLeftIndex(int nodeIndex) const {
    int result = 2 * nodeIndex + 1;
    if (!(0 <= result && result <= (m_actualSize-1))) // make sure this index is valid
        return -1;
    return result;
}

/*
 * returns the index of the right child node of the node at nodeIndex
 */
template<class T, int m_size>
int Heap<T, m_size>::GetRightIndex(int nodeIndex) const {
    int result = 2 * nodeIndex + 2;
    if (!(0 <= result && result <= (m_actualSize-1))) // make sure this index is valid
        return -1;
    return result;
}

/*
 * returns the index of the parent node of the node at nodeIndex
 */
template<class T, int m_size>
int Heap<T, m_size>::GetParentIndex(int nodeIndex) const {
    int result = (nodeIndex - 1) / 2; // c++ automatically rounds down to an integer
    if (!(0 <= result && result <= (m_actualSize-1))) // make sure this index is valid
        return -1;
    return result;
}

template <class T, int m_size>
void Heap<T, m_size>::LevelPrint() const {
    cout << "_____Level order print_____" << endl;
    for (int i=0; i<m_actualSize; i++) {
        cout << i << ": " << m_array[i].GetKey() << ", " << m_array[i].GetValue() << endl;
    }

    cout << "___________________________\n\n";
}

#endif /* HEAP_CPP_ */
