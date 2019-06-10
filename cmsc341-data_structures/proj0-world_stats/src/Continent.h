#ifndef CONTINENT_H
#define CONTINENT_H

#include <iostream>
#include <vector>
#include "Country.h"
using namespace std;

class Continent : public Country {
	public:
		vector<Country> getCountriesInContinent() const;
		Country getHighestPopulation() const;
		Country getHighestGDPSpent() const;
		Country getHighestLiteracyRate() const;

		void setCountriesInContinent(vector<Country> val);
		void setHighestPopulation(Country val);
		void setHighestGDPSpent(Country val);
		void setHighestLiteracyRate(Country val);

		friend ostream& operator<<(ostream& sout, const Continent& c);

	private:
		vector<Country> countriesInContinent;
		Country highestPopulation;
		Country highestGDPSpent;
		Country highestLiteracyRate;
		// Continent also inherits the variables/methods in Country
};

#endif
