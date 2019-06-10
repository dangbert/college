#ifndef _AVLTREE_CPP_
#define _AVLTREE_CPP_

#include "AVLTree.h"
#include <stdlib.h>
#include <iostream>
#include <queue>
using std::cout;
using std::endl;

template <class T, class N>
AVLTree<T, N>::AVLTree(int printOrder) {
    m_root = NULL;

    if (printOrder == 0) m_printOrder = AVLTreeBase::IN;
    else if (printOrder == 1) m_printOrder = AVLTreeBase::PRE;
    else if (printOrder == 2) m_printOrder = AVLTreeBase::POST;
    else m_printOrder = AVLTreeBase::LEVEL;
}

template <class T, class N>
AVLTree<T, N>::~AVLTree() {
    if (m_root != NULL) {
        delete m_root; // calls AVLNode destructor
    }
}

/*
 * public function to insert data into the tree
 * (so that a node doesn't have to be created by the code using this class)
 * this function calls AVL_insert() to recursively insert the data
 */
template <class T, class N>
void AVLTree<T, N>::insert(T data, N num) {
    m_root = AVL_insert(m_root, data, num);
}

/*
 * recursively inserts a new node with the given data/num in the AVL subtree below node
 * this function is used by insert()
 */
template <class T, class N>
AVLNode<T, N>* AVLTree<T, N>::AVL_insert(AVLNode<T, N>* node, T data, N num) {

    if (node == NULL) {
        //cout << "  inserting new node here" << endl;
        return new AVLNode<T, N>(data, num);
    }
    if (num <= node->m_num) {
        //cout << "  continuing insert of (" << num << ") left of " << node->m_num << endl;
        node->m_left = AVL_insert(node->m_left, data, num);
    }
    else {
        //cout << "  continuing insert of (" << num << ") right of " << node->m_num << endl;
        node->m_right = AVL_insert(node->m_right, data, num);
    }

    return balance(node); // balance the subtree with node as the root
}

/*
 * balances the tree with the given root node (so that it is an AVL tree)
 */
template <class T, class N>
AVLNode<T, N>* AVLTree<T, N>::balance(AVLNode<T, N>* node) {
    updateHeight(node); // update height now that a node has been inserted
    int diff = getHeightDiff(node);
    //cout << "      this node has height = " << getHeight(node) << ",  diff = " << diff << endl << endl;

    if (diff > 1) { // m_right has too many nodes
        //cout << "   ---------------------" << endl;
        if (getHeightDiff(node->m_right) < 0)
            node->m_right = rotateRight(node->m_right);
        //cout << "   ---------------------" << endl;
        return rotateLeft(node);
    }

    if (diff < -1) { // m_left has too many nodes

        //cout << "   ---------------------" << endl;
        if (getHeightDiff(node->m_left) > 0)
            node->m_left = rotateLeft(node->m_left);
        //cout << "   ---------------------" << endl;
        return rotateRight(node);
    }
    return node;
}

/*
 * returns the height difference between the right and left subtree of
 * the given node
 */
template <class T, class N>
int AVLTree<T, N>::getHeightDiff(AVLNode<T, N>* node) {
    //cout << "    getting height diff of (" << node->m_num << "). heights: left: " << getHeight(node->m_left);
    //cout << ", right: " << getHeight(node->m_right) << endl;
    return getHeight(node->m_right) - getHeight(node->m_left);
}

/*
 * returns the height of a given node (0 if it's a leaf)
 * (assumes height of node is stored correctly)
 * (the height of a node is the max of the subtree heights + 1)
 */
template <class T, class N>
int AVLTree<T, N>::getHeight(AVLNode<T, N>* node) {
    if (node == NULL) {
        return 0;
    }
    return node->m_height;
}

/*
 * update the height of a given node
 * (assumes child nodes have correct height)
 */
template <class T, class N>
void AVLTree<T, N>::updateHeight(AVLNode<T, N>* node) {
    int leftHeight = getHeight(node->m_left); // height of left node
    int rightHeight = getHeight(node->m_right);

    // make node's height equal to the height of the largest subtree + 1
    if (leftHeight >= rightHeight)
        node->m_height = leftHeight + 1;
    else
        node->m_height = rightHeight + 1;

}

/*
 * perform a single left rotation on the subtree with node as the root
 */
template <class T, class N>
AVLNode<T, N>* AVLTree<T, N>::rotateLeft(AVLNode<T, N>* node) {
    //cout << " rotating left" << endl;
    AVLNode<T, N>* new_root = node->m_right; // new root of this tree

    node->m_right = new_root->m_left;
    new_root->m_left = node;

    // fix heights
    updateHeight(node);
    updateHeight(new_root);

    return new_root;
}

/*
 * perform a single right rotation on the subtree with node as the root
 * (opposite of leftRotate function)
 */
template <class T, class N>
AVLNode<T, N>* AVLTree<T, N>::rotateRight(AVLNode<T, N>* node) {
    //cout << " rotating right" << endl;
    AVLNode<T, N>* new_root = node->m_left; // new root of this tree

    node->m_left = new_root->m_right;
    new_root->m_right = node;

    // fix heights
    updateHeight(node);
    updateHeight(new_root);

    return new_root;
}

/*
 * public function that prints the nodes of the tree that are in the given range
 * (inclusive) in order according to m_printOrder
 */
template <class T, class N>
void AVLTree<T, N>::print_range(std::ostream& fstream, N lowerBound, N upperBound) {
    AVL_printAll(fstream, m_root, true, lowerBound, upperBound);
    fstream << endl;
}

/*
 * public function that prints all of the tree according to m_printOrder
 */
template <class T, class N>
void AVLTree<T, N>::printAll(std::ostream& fstream) {
    AVL_printAll(fstream, m_root, false, 0, 0);
    fstream << endl;
}

/*
 * recursively print tree according to m_printOrder
 * if printRange == true, then only print nodes where m_num is between
 * lowerBound and upperBound (inclusive)
 */
template <class T, class N>
void AVLTree<T, N>::AVL_printAll(std::ostream& fstream, AVLNode<T,N>* node, bool printRange, N lowerBound, N upperBound) {
    // in order print
    if (m_printOrder == AVLTreeBase::IN) {

        if (node != NULL) {
            AVL_printAll(fstream, node->m_left, printRange, lowerBound, upperBound);

            if (printRange) {
                if (lowerBound <= node->m_num && node->m_num <= upperBound)
                    fstream << node->m_data << " ";
            }
            else
                fstream << node->m_data << " ";

            AVL_printAll(fstream, node->m_right, printRange, lowerBound, upperBound);
        }
    }

    // pre order print
    else if (m_printOrder == AVLTreeBase::PRE) {

        if (node != NULL) {
            if (printRange) {
                if (lowerBound <= node->m_num && node->m_num <= upperBound)
                    fstream << node->m_data << " ";
            }
            else
                fstream << node->m_data << " ";

            AVL_printAll(fstream, node->m_left, printRange, lowerBound, upperBound);
            AVL_printAll(fstream, node->m_right, printRange, lowerBound, upperBound);
        }
    }

    // post order print
    else if (m_printOrder == AVLTreeBase::POST) {

        if (node != NULL) {
            AVL_printAll(fstream, node->m_left, printRange, lowerBound, upperBound);
            AVL_printAll(fstream, node->m_right, printRange, lowerBound, upperBound);

            if (printRange) {
                if (lowerBound <= node->m_num && node->m_num <= upperBound)
                    fstream << node->m_data << " ";
            }
            else
                fstream << node->m_data << " ";
        }
    }

    // level order print
    else if (m_printOrder == AVLTreeBase::LEVEL) {
        std::queue< AVLNode<T, N>* > q; // a queue is first in first out

        if (node != NULL) {
            q.push(node);
        }

        while (!q.empty()) {
            // get the node at the front of queue and pop it from the queue
            AVLNode<T, N>* temp = q.front();
            q.pop();

            // output the data stored in temp
            if (printRange) {
                if (lowerBound <= temp->m_num && temp->m_num <= upperBound)
                    fstream << temp->m_data << " ";
            }
            else {
                fstream << temp->m_data << " ";
            }

            // push temp's left and right nodes into the queue
            if (temp->m_left != NULL) {
                q.push(temp->m_left);
            }
            if (temp->m_right != NULL) {
                q.push(temp->m_right);
            }
        }
    }

}

#endif
