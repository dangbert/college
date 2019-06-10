#ifndef _AVLNODE_H_
#define _AVLNODE_H_

#include "AVLTree.h"

/*
 * T is the class type for m_data (char or string)
 * N is the class type for m_num (int or float)
 */
template <class T, class N>
class AVLNode {
    friend class AVLTree<T, N>;

    public:
        AVLNode(T data, N num);
        ~AVLNode();

    private:
        T m_data; // data stored in node
        N m_num; // number stored in node (for sorting)
        int m_height; // height of this node (# of nodes on longest path from here to a leaf)

        AVLNode<T, N>* m_left; // pointer to left subtree's root node
        AVLNode<T, N>* m_right; // pointer to right subtree's root node
};

#include "AVLNode.cpp"
#endif
