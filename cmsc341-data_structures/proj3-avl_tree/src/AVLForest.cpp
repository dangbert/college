#include "AVLForest.h"
#include <sstream>
#include <fstream>
using namespace std; // TODO: remove this line later?
using std::cout;
using std::endl;

AVLForest::AVLForest() {

}

/*
 * destructor:
 * static cast each AVLTreeBase pointer and call it's destructor explicitly
 * (have to do this because there's no base virtual destructor and I'm not
 * allowed to modify AVLTreeBase.h)
 */
AVLForest::~AVLForest() {
    // iterate over map and delete each associated tree
    for (std::map< int, std::vector<int> >::const_iterator it = m_ids.begin(); it!=m_ids.end(); ++it) {
        // delete tree with the ID it->first
        vector<int> info = it->second; // get info vector for current tree
        int index = info.at(0); // get index in m_forest of pointer to tree
        int dataType = info.at(1); // data type
        int numType = info.at(2); // number type

        // character and integer
        if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::INTEGER) {
            AVLTree<char, int>* tree = static_cast<AVLTree<char, int>*>(m_forest.at(index));
            delete tree;
        }

        // character and float
        else if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::FLOAT) {
            AVLTree<char, float>* tree = static_cast<AVLTree<char, float>*>(m_forest.at(index));
            delete tree;
        }

        // string and integer
        else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::INTEGER) {
            AVLTree<string, int>* tree = static_cast<AVLTree<std::string, int>*>(m_forest.at(index));
            delete tree;
        }

        // string and float
        else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::FLOAT) {
            AVLTree<string, float>* tree = static_cast<AVLTree<std::string, float>*>(m_forest.at(index));
            delete tree;
        }
    }
}

/*
 * @param line: 1 line from an input file
 * line formats (new tree or insert into tree):
 * new tree format: <AVL tree ID,order of printing(enum),data type (enum),number type (enum)>
 * insert format: <AVL tree ID,T1,T2>
 */
void AVLForest::insert(std::string line) {
    //cout << "line: '" << line << "'" << endl;
    std::stringstream ss(line);
    vector<string> values; // vector to store parts of line in
    while (ss.good()) {
        string temp;
        getline(ss, temp, ',');
        values.push_back(temp);
    }

    int id = makeInt(values.at(0)); // store tree ID as int

    // if this is a new tree
    if (values.size() == 4) {
        // create a new tree and add it to the vector
        //cout << "Tree doesn't exist" << endl;

        int printOrder = makeInt(values.at(1)); // print order
        int dataType = makeInt(values.at(2)); // data type
        int numType = makeInt(values.at(3)); // number type

        AVLTreeBase* tree = NULL;

        // character and integer
        if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::INTEGER) {
            tree = new AVLTree<char, int>(printOrder);
        }

        // character and float
        else if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::FLOAT) {
            tree = new AVLTree<char, float>(printOrder);
        }

        // string and integer
        else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::INTEGER) {
            tree = new AVLTree<std::string, int>(printOrder);
        }

        // string and float
        else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::FLOAT) {
            tree = new AVLTree<std::string, float>(printOrder);
        }

        else {
            // invalid case
        }

        m_forest.push_back(tree); // store tree pointer in m_forest
        m_order.push_back(id); // store tree id in m_order
        int index = (m_forest.size()-1); // index of tree in m_forest

        vector<int> info; // vector of tree info
        info.push_back(index);
        info.push_back(dataType);
        info.push_back(numType);

        m_ids.insert(std::make_pair(id, info)); // map id to info vector
    }

    else { // tree already exists so insert data, number pair
        vector<int> info = getInfo(id); // get info vector for tree with given id
        if (info.size() == 0) {
            // tree with this id doesn't exist
            cout << "****error tree with id=" << id << " not found*****" << endl;
            return;
        }

        int index = info.at(0); // get index in m_forest of pointer to tree
        int dataType = info.at(1); // data type
        int numType = info.at(2); // number type

        // character and integer
        if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::INTEGER) {
            // static_cast to <AVLTree<char, int>*>
            AVLTree<char, int>* tree = static_cast<AVLTree<char, int>*>(m_forest.at(index));

            char data = values.at(1)[0]; // convert string to char
            int num = makeInt(values.at(2));
            tree->insert(data, num);
        }

        // character and float
        else if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::FLOAT) {
            AVLTree<char, float>* tree = static_cast<AVLTree<char, float>*>(m_forest.at(index));

            char data = values.at(1)[0];
            float num = makeFloat(values.at(2));
            tree->insert(data, num);
        }

        // string and integer
        else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::INTEGER) {
            AVLTree<string, int>* tree = static_cast<AVLTree<std::string, int>*>(m_forest.at(index));

            string data = values.at(1);
            int num = makeInt(values.at(2));
            tree->insert(data, num);
        }

        // string and float
        else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::FLOAT) {
            AVLTree<string, float>* tree = static_cast<AVLTree<std::string, float>*>(m_forest.at(index));

            string data = values.at(1);
            float num = makeFloat(values.at(2));
            tree->insert(data, num);
        }

        else {
        // invalid case
            cout << "****error mismatched types: tree id=" << id << " *****" << endl;
            return;
        }
    }

    //cout << "_______\n\n";
}

/*
 * print the data (in nodes within the specified range) in the tree with given id
 * line format: <AVL tree ID,lower limit, upper limit>
 */
void AVLForest::print_range(std::ostream& fstream, std::string line) {
    std::stringstream ss(line);
    vector<string> values; // vector to store parts of line in (as strings)
    while (ss.good()) {
        string temp;
        getline(ss, temp, ',');
        values.push_back(temp);
    }

    int id = makeInt(values.at(0)); // store tree ID as int
    vector<int> info = getInfo(id); // get info vector for tree with given id

    if (info.size() == 0) {
        // tree with this id doesn't exist
        cout << "****error tree with id=" << id << " not found*****" << endl;
        return;
    }

    int index = info.at(0); // get index in m_forest of pointer to tree
    int dataType = info.at(1); // data type
    int numType = info.at(2); // number type

    // values.at(1)) is the lower bound (stored as a string)
    // values.at(1)) is the upper bound (stored as a string)

    // static cast to the AVLTree with the appropriate templated types:
    // character and integer
    if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::INTEGER) {
        AVLTree<char, int>* tree = static_cast<AVLTree<char, int>*>(m_forest.at(index));
        tree->print_range(fstream, makeInt(values.at(1)), makeInt(values.at(2)));
    }
    else if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::FLOAT) {
        AVLTree<char, float>* tree = static_cast<AVLTree<char, float>*>(m_forest.at(index));
        tree->print_range(fstream, makeFloat(values.at(1)), makeFloat(values.at(2)));
    }
    else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::INTEGER) {
        AVLTree<string, int>* tree = static_cast<AVLTree<std::string, int>*>(m_forest.at(index));
        tree->print_range(fstream, makeInt(values.at(1)), makeInt(values.at(2)));
    }
    else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::FLOAT) {
        AVLTree<string, float>* tree = static_cast<AVLTree<std::string, float>*>(m_forest.at(index));
        tree->print_range(fstream, makeFloat(values.at(1)), makeFloat(values.at(2)));

    }
}

/*
 * prints all trees in the vector (in sequence in which they were first
 * encountered in the input file)
 */
std::ostream& operator << (std::ostream& out, AVLForest& avlForest) {
    // print all trees
    // loop through ids of trees (in order encounted in input file)
    for (uint i=0; i<avlForest.m_order.size(); i++) {
        int id = avlForest.m_order.at(i); // id of current tree
        vector<int> info = avlForest.getInfo(id); // get info vector for tree with given id

        int index = info.at(0); // get index in m_forest of pointer to tree
        int dataType = info.at(1); // tree data type
        int numType = info.at(2); // tree number type

        // character and integer
        if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::INTEGER) {
            // static_cast to <AVLTree<char, int>*>
            AVLTree<char, int>* tree = static_cast<AVLTree<char, int>*>(avlForest.m_forest.at(index));
            tree->printAll(out);
        }

        // character and float
        else if (dataType == AVLTreeBase::CHARACTER && numType == AVLTreeBase::FLOAT) {
            AVLTree<char, float>* tree = static_cast<AVLTree<char, float>*>(avlForest.m_forest.at(index));
            tree->printAll(out);
        }

        // string and integer
        else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::INTEGER) {
            AVLTree<string, int>* tree = static_cast<AVLTree<std::string, int>*>(avlForest.m_forest.at(index));
            tree->printAll(out);
        }

        // string and float
        else if (dataType == AVLTreeBase::STRING && numType == AVLTreeBase::FLOAT) {
            AVLTree<string, float>* tree = static_cast<AVLTree<std::string, float>*>(avlForest.m_forest.at(index));
            tree->printAll(out);
        }

        else {
            // invalid case
        }
    }


    return out;
}

/*
 * return vector<int> of important info for the tree with the given id
 * vector format: {m_forest_index, dataType, numType}
 * returns empty vector if the tree with the given id doesn't exist
 */
std::vector<int> AVLForest::getInfo(int id) const {
    // must use const_iterator because the function is const
    std::map< int, std::vector<int> >::const_iterator it = m_ids.find(id);
    if (it == m_ids.end()) {
        // not found
        cout << "ERROR " << id << " ---> not found" << endl;
        vector<int> empty;
        return empty; // return empty vector
    }
    return it->second;
}

// convert string s to an int
int AVLForest::makeInt(std::string s) const {
    return atoi(s.c_str());
}

// convert string s to a float
float AVLForest::makeFloat(std::string s) const {
    return atof(s.c_str());
}
