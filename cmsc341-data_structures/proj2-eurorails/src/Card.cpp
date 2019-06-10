#include "Card.h"
#include <stdlib.h>
using namespace std;


/*
 * destructor
 * free memory in m_objectives
 */
Card::~Card() {
    for (uint i=0; i<m_objectives.size(); i++) {
        delete m_objectives.at(i);
    }
}

/*
 * inserts an objective into the card
 */
void Card::addObjective(Objective* objective) {
    m_objectives.push_back(objective);
}

/*
 * returns a pointer to the objective on the card with the highest payoff
 * assumes m_objectives.size() > 0
 */
Objective* Card::getBestPayoff() {
    if (m_objectives.size() == 0)
        return NULL;
    Objective* temp = m_objectives.at(0);
    for (uint i=0; i<m_objectives.size(); i++) {
        if (m_objectives.at(i)->getPayoff() > temp->getPayoff()) {
            temp = m_objectives.at(i);
        }
    }
    return temp;
}

/*
 * returns a pointer to the objective that has the first alphabetic
 * destination name. if two objectives have the same destination, the one that
 * appears first on the card will be selected.
 */
Objective* Card::getFirstAlphaDestination() {
    if (m_objectives.size() == 0)
        return NULL;
    Objective* temp = m_objectives.at(0);
    for (uint i=0; i<m_objectives.size(); i++) {
        // if temp's destination comes before i's (in alphabetical order)
        if (m_objectives.at(i)->getDestination() < temp->getDestination()) {
            temp = m_objectives.at(i);
        }
    }
    return temp;
}

/*
 * returns a pointer to the objective with a commodity whose color matches
 * the requested color.
 * if multiple have the same color, return the one of those with the highest payoff
 * if no commodities on the card have that color, return the one with the highest payoff
 */
Objective* Card::getByColor(Commodity::COLOR color) {
    if (m_objectives.size() == 0)
        return NULL;
    int found = 0; // whether a matching card has been found yet
    Objective* temp;
    for (uint i=0; i<m_objectives.size(); i++) {
        // if color matches
        if (m_objectives.at(i)->getCommodity()->getColor() == color) {
            if (!found) { // when this is the first matching card found
                temp = m_objectives.at(i);
                found = 1;
            }
            // if another matching card that has a higher payoff is found
            else if (m_objectives.at(i)->getPayoff() > temp->getPayoff()) {
                temp = m_objectives.at(i);
            }
        }
    }
    if (found) {
        return temp;
    }
    return getBestPayoff(); // return highest payoff instead
}

/*
 * prints the card with its objectives in the order they were read in from
 * the file to the given output stream
 */
void Card::printCard(std::ofstream& fileStream) {
    fileStream << "---------- CARD ----------" << endl;
    for (uint i=0; i<m_objectives.size(); i++) {
        m_objectives.at(i)->printObjective(fileStream);
    }
    fileStream << "--------------------------" << endl;
}
