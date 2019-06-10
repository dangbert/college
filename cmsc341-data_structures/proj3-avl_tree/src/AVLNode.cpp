#ifndef _AVLNODE_CPP_
#define _AVLNODE_CPP_

#include "AVLNode.h"
#include <stdlib.h>
#include <iostream>

/*
 * construct a new node that stores data and num
 */
template <class T, class N>
AVLNode<T, N>::AVLNode(T data, N num) {
    m_data = data;
    m_num = num;
    m_left = NULL;
    m_right = NULL;
    m_height = 1; // node has a height of 1 by default
}

template <class T, class N>
AVLNode<T, N>::~AVLNode() {
    if (m_left != NULL)
        delete m_left;
    if (m_right != NULL)
        delete m_right;
}

#endif
