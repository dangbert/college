#include "CommodityStore.h"
#include "Commodity.h"
#include <fstream>
#include <stdlib.h>
using namespace std;


/*
 * destructor
 * frees memory from Commodity pointers in m_store
 */
CommodityStore::~CommodityStore() {
    for (std::map<std::string, Commodity*>::iterator it=m_store.begin(); it!=m_store.end(); it++) {
        delete it->second; // delete Commodity object
    }
}

/*
 * reads in the provided commodities text file and creates an instance
 * for each commodity which it stores in m_store
 * @param filename: the name of the commodities text file
 */
void CommodityStore::loadCommodities(std::string filename) {
    ifstream ifile(filename.c_str());
    string name; // commodity name
    string color; // commodity color
    Commodity::COLOR eColor; // enum color

    while (!ifile.eof()) {
        ifile >> name; // store next string in file in name
        ifile >> color;
        if (ifile.eof()) // check that we haven't reached the end of the file
            break;

        if (color == "Purple") eColor = Commodity::PURPLE;
        if (color == "Red") eColor = Commodity::RED;
        if (color == "Orange") eColor = Commodity::ORANGE;
        if (color == "Black") eColor = Commodity::BLACK;
        if (color == "Green") eColor = Commodity::GREEN;
        if (color == "Brown") eColor = Commodity::BROWN;
        if (color == "Colorless") eColor = Commodity::COLORLESS;

        Commodity* c = new Commodity(name, eColor);
        m_store.insert(std::make_pair(name, c));
    }
}

/*
 * returns the commodity instance corresponding to the provided string name
 */
Commodity* CommodityStore::getCommodity(std::string name) {
    std::map<std::string, Commodity*>::iterator it = m_store.find(name);
    if (it == m_store.end()) {
        return NULL;
    }
    return it->second;
}
