#ifndef _OBJECTIVE_H_
#define _OBJECTIVE_H_

#include <iostream>
#include <fstream>
#include "Commodity.h"
using namespace std;


class Objective {
    public:
        Objective(std::string destination, Commodity* c, int payoff);
        std::string getDestination() const;
        Commodity* getCommodity() const;
        int getPayoff() const;
        void printObjective(std::ofstream& fileStream);

    private:
        std::string m_sDestination; // name of destination for the commodity to be delivered to
        Commodity* m_pCommodity; // pointer to the Commodity in the CommodityStore being requested in this objective
        int m_iPayoff; // amount of points player will receive for delivering this commodity to its destination

};

#endif
