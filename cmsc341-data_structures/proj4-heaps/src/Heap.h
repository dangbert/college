#ifndef HEAP_H_
#define HEAP_H_

/*
 * Abstract heap class
 * the functions NeedSwap and Compare must be implemented by child classes
 */
template<class T, int m_size>
class Heap {

    public:
        // required functions by UML
        Heap();
        Heap(const Heap<T, m_size>& origHeap);
        bool Contains(const T& needle) const;
        const T* Find(const T& needle) const;
        T& Remove();
        void Insert(T& insertable);
        void PercolateUp(int index);
        void PercolateDown(int index);

        // student-made functions
        virtual ~Heap();
        void LevelPrint() const;

    protected:
        // UML-required member variables
        T* m_array;

        // student-made member variables
        int m_actualSize; // number of objects currently stored in the heap

        // student-made functions
        virtual bool NeedSwap(int parentIndex, int childIndex) = 0; // pure virtual
        virtual int Compare(int leftIndex, int rightIndex) = 0; // pure virtual

        bool Swap(int parentIndex, int childIndex);
        int GetLeftIndex(int nodeIndex) const;
        int GetRightIndex(int nodeIndex) const;
        int GetParentIndex(int nodeIndex)const ;
};

#include "Heap.cpp"
#endif /* HEAP_H_ */
