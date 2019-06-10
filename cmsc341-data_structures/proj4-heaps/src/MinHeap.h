#ifndef MIN_HEAP_H_
#define MIN_HEAP_H_

#include "Heap.h"

/*
 * MinHeap class (derives from the Heap class)
 * functions as a Min Heap data structure
 */
template <class T, int m_size>
class MinHeap : public Heap<T, m_size> {

    public:
        MinHeap();
        MinHeap(const Heap<T, m_size>& origHeap);
           
    private:
        bool NeedSwap(int parentIndex, int childIndex);
        int Compare(int leftIndex, int rightIndex);
};

#include "MinHeap.cpp"
#endif
