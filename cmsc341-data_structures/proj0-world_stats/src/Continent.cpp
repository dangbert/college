#include <stdlib.h>
#include "Continent.h"

void Continent::setCountriesInContinent(vector<Country> val) {
	countriesInContinent = val;

    long pop = 0; // total population of this continent
    uint maxPop = 0; // index of country with max population
    uint maxGDP = 0; // index of country with max GDP spent on education
    uint maxLit = 0; // index of country with max literacy rate


    for (uint i=0; i<countriesInContinent.size(); i++) {
        if (countriesInContinent.at(i).getPopulation() != 0)
            pop += countriesInContinent.at(i).getPopulation();

        if (countriesInContinent.at(i).getPopulation() > countriesInContinent.at(maxPop).getPopulation()) {
            maxPop = i;
        }

        if (countriesInContinent.at(i).getEducationGDPSpent() > countriesInContinent.at(maxGDP).getEducationGDPSpent()) {
            maxGDP = i;
        }

        if (countriesInContinent.at(i).getLiteracyRate() > countriesInContinent.at(maxLit).getLiteracyRate()) {
            maxLit = i;
        }
    }

    setPopulation(pop);
    setHighestPopulation(countriesInContinent.at(maxPop));
    setHighestGDPSpent(countriesInContinent.at(maxGDP));
    setHighestLiteracyRate(countriesInContinent.at(maxLit));
}

void Continent::setHighestPopulation(Country val) {
	highestPopulation = val;
}

void Continent::setHighestGDPSpent(Country val) {
	highestGDPSpent = val;
}

void Continent::setHighestLiteracyRate(Country val) {
	highestLiteracyRate = val;
}


vector<Country> Continent::getCountriesInContinent() const {
	return countriesInContinent;
}

Country Continent::getHighestPopulation() const {
	return highestPopulation;
}

Country Continent::getHighestGDPSpent() const {
	return highestGDPSpent;
}

Country Continent::getHighestLiteracyRate() const {
	return highestLiteracyRate;
}


ostream& operator<<(ostream& sout, const Continent& c) {
    sout << c.getName() << endl;
    sout << " Population: " << c.getPopulation() << endl;

	Country lit = c.getHighestLiteracyRate();
	sout << " Country with highest literacy rate: " << lit.getName() << " with a population of ";
	sout << lit.getPopulation() << " and a literacy rate of " << lit.getLiteracyRate() << endl;

	Country gdp = c.getHighestGDPSpent();
	sout << " Country with highest GDP expenditure on education: " << gdp.getName() << " with a population of ";
	sout << gdp.getPopulation() << " and a literacy rate of " << gdp.getLiteracyRate() << endl;

	Country pop = c.getHighestPopulation();
	sout << " Country with highest population: " << pop.getName() << " with a population of ";
	sout << pop.getPopulation() << " and a literacy rate of " << pop.getLiteracyRate() << endl;

	return sout;
}
