#include "Commodity.h"
using namespace std;


/*
 * constructor
 */
Commodity::Commodity(string name, COLOR color)
    : m_sName(name), m_eColor(color)
{

}

/*
 * returns a copy of the string name of the commodity
 */
std::string Commodity::getName() const {
    return m_sName;
}

/*
 * returns the color index of the commodity
 */
Commodity::COLOR Commodity::getColor() {
    return m_eColor;
}
