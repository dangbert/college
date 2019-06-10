#ifndef MAX_HEAP_H_
#define MAX_HEAP_H_

#include "Heap.h"

/*
 * MaxHeap class (derives from the Heap class)
 * functions as a Max Heap data structure
 */
template <class T, int m_size>
class MaxHeap : public Heap<T, m_size> {

    public:
        MaxHeap();
        MaxHeap(const Heap<T, m_size>& origHeap);
           
    private:
        bool NeedSwap(int parentIndex, int childIndex);
        int Compare(int leftIndex, int rightIndex);
};

#include "MaxHeap.cpp"
#endif
