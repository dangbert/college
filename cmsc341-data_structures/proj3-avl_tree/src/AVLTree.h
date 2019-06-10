#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "AVLTreeBase.h"
#include <fstream>

// class prototypes so node and tree classes know that each other is a class
template <class T, class N> class AVLTree;
template <class T, class N> class AVLNode;
#include "AVLNode.h"

/*
 * inherits from AVLTreeBase
 */
template <class T, class N>
class AVLTree : public AVLTreeBase {
    public:
        AVLTree(int printOrder);
        ~AVLTree();
        void insert(T data, N num);
        void print_range(std::ostream& fstream, N lowerBound, N upperBound);
        void printAll(std::ostream& fstream);
        void AVL_printAll(std::ostream& fstream, AVLNode<T, N>* node, bool printRange, N lowerBound, N upperBound);

    private:
        AVLNode<T, N>* m_root; // pointer to root node of this tree
        PrintOrder m_printOrder; // (enum) type of print order for this tree

        AVLNode<T, N>* AVL_insert(AVLNode<T, N>* node, T data, N num);
        AVLNode<T, N>* balance(AVLNode<T, N>* node);
        int getHeightDiff(AVLNode<T, N>* node);
        int getHeight(AVLNode<T, N>* node);
        void updateHeight(AVLNode<T, N>* node);
        AVLNode<T, N>* rotateLeft(AVLNode<T, N>* node);
        AVLNode<T, N>* rotateRight(AVLNode<T, N>* node);
};

#include "AVLTree.cpp"
#endif
