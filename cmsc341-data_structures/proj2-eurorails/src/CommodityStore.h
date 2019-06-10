#ifndef _COMMODITYSTORE_H_
#define _COMMODITYSTORE_H_

#include <iostream>
#include <map>
#include "Commodity.h"
using namespace std;


class CommodityStore {
    public:
        ~CommodityStore();
        void loadCommodities(std::string filename);
        Commodity* getCommodity(std::string name);


    private:
        std::map<std::string, Commodity*> m_store; // maps instances of each commodity to their string name

};

#endif
