#include "Objective.h"
using namespace std;


/*
 * constructor
 */
Objective::Objective(std::string destination, Commodity* c, int payoff)
    : m_sDestination(destination), m_pCommodity(c), m_iPayoff(payoff)
{

}

/*
 * returns a copy of the name of the destination
 */
std::string Objective::getDestination() const {
    return m_sDestination;
}

/*
 * returns pointer to the requested commodity
 */
Commodity* Objective::getCommodity() const {
    return m_pCommodity;
}

/*
 * returns a copy of the value received for delivering this
 * commodity to its destination
 */
int Objective::getPayoff() const {
    return m_iPayoff;
}

/*
 * prints an objective (to the proveided output stream)
 * as selected by a player in the simulation
 * format: “<commodity> to <destination> for <payoff>”
 */
void Objective::printObjective(std::ofstream& fileStream) {
	fileStream << getCommodity()->getName();
	fileStream << " to " << getDestination();
	fileStream << " for " << getPayoff() << endl;

}
