#ifndef _CARD_H_
#define _CARD_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "Objective.h"
#include "Commodity.h"
using namespace std;


class Card {
    public:
        ~Card();
        void addObjective(Objective* objective);
        Objective* getBestPayoff();
        Objective* getFirstAlphaDestination();
        Objective* getByColor(Commodity::COLOR color);
        void printCard(std::ofstream& fileStream);

    private:
        std::vector<Objective*> m_objectives; // contains the card's objectives in the order read from file

};

#endif
