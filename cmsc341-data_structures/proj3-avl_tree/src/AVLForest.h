#ifndef _AVLFOREST_H_
#define _AVLFOREST_H_

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stdlib.h>
#include "AVLTreeBase.h"
#include "AVLTree.h"

class AVLForest
{
    public:
        AVLForest();
        ~AVLForest();

        void insert(std::string);
        void print_range(std::ostream&, std::string);
        friend std::ostream& operator << (std::ostream&, AVLForest&);

    private:
    // we use a non-templated base class to hold all template instantiations
    std::vector<AVLTreeBase*> m_forest; // vector of trees
    std::vector<int> m_order; // vector of tree ids in the order read from input file

    std::map< int, std::vector<int> > m_ids; // map tree ids to important info
    std::vector<int> getInfo(int id) const; // get info vector for tree
    int makeInt(std::string s) const;
    float makeFloat(std::string s) const;
};

#endif
