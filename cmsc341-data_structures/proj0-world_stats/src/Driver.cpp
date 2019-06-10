#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "Country.h"
#include "Continent.h"

using namespace std;

vector<Continent> readFromFiles(string file1, string file2);
long makeLong(string s);
float makeFloat(string s);
void printCountry(Country c);
void printWorld(vector<Continent> world);

int main() {
    string file1 = "2013WorldBankEducationCensusData.txt";
    string file2 = "CountriesContinents.txt";
    vector<Continent> world = readFromFiles(file1, file2); // parse the data files and create vector of continent objects

    // output Continent stats
    for (uint n=0; n<world.size(); n++) {
        cout << world.at(n);
    }

    return 0;
}

/**
 * reads from data files and creates a vector of Continent objects which contain
 * a vector of Country objects storing statistics about that country
 *
 * @param file1 name of the file containing statistics about each country
 * @param file2 name of the file containing a list of the countries in each continent
 * @return vector of Continent objects
 */
vector<Continent> readFromFiles(string file1, string file2) {
    vector<Country> allCountries; // vector to store all parsed Country objects in
    Country c; // current country object
    string s;

    ifstream ifile1(file1.c_str()); // file containing statistics for each country
    getline(ifile1, s); // skip first line of file because it contains no data

    // parse through stat file and create a country object for each country and store them in allCountries
    while (!ifile1.eof()) {

        for (int i=0; i<9; i++) {
            ifile1 >> s; // store the next string in the file in s
            if (ifile1.eof()) { // check that we haven't reached the end of the file
                break;
            }

            if (i == 0) { // if the first string on the (tab delimted) line
                c = Country();
                c.setName(s);
            }
            if (i == 1)
                c.setPopulation(makeLong(s));
            if (i == 2)
                c.setLiteracyRate(makeFloat(s));
            if (i == 3)
                c.setEducationGDPSpent(makeFloat(s));
            if (i == 4)
                c.setPrimaryCompletionTotal(makeFloat(s));
            if (i == 5)
                c.setPrimaryCompletionMale(makeFloat(s));
            if (i == 6)
                c.setPrimaryCompletionFemale(makeFloat(s));
            if (i == 7)
                c.setYouthLitRateFem(makeFloat(s));
            if (i == 8) { // if the last string on the line
                c.setYouthLitRateMale(makeFloat(s));
                allCountries.push_back(c); // store the completed Country in allCountries
            }
        }
    }
    ifile1.close(); // close the file stream


    vector<Continent> world; // vector to store all the Continent objects
    Continent curContinent; // current Continent object
    vector<Country> curCountries; // vector of Country objects for the current continent

    ifstream ifile2(file2.c_str()); // file listing the countries in each continent

    // parse the continents file and copy countries in allCountries to respective Continent objects
    while (!ifile2.eof()) {
        getline(ifile2, s);
        c = Country(); // current country

        // if start of continent in the file
        if (s.find("--") != string::npos) {
            if (world.size() > 0 || curCountries.size() > 0) { // if not the first continent found
                curContinent.setCountriesInContinent(curCountries); // store countries in the last continent
                world.push_back(curContinent); // add the last continent to world
                curCountries.clear(); // empty curCountries
            }

            curContinent = Continent(); // create new Continent object
            curContinent.setName(s.substr(0, s.find(" "))); // set the name of the continent
        }
        else if (!s.empty()) { // s is the country's name
            int found = 0;

            // here get the country from allCountries with name == s
            for (uint i=0; i<allCountries.size(); i++) {
                if (allCountries.at(i).getName() == s) {
                    curCountries.push_back(allCountries.at(i)); // store this country in the vector of countries

                    found = 1;
                    break;
                }
            }
            if (!found) {
                //cout << "ERROR: Unable to find country '" << s << "'" << endl;
            }
        }

    }
    curContinent.setCountriesInContinent(curCountries); // store countries from the last continent
    world.push_back(curContinent); // add last continent to world
    ifile2.close();

    return world; // return the vector of Continents
}


/**
 * Converts a string to a long
 * @param s the string to be converted
 * @return long, -1 if the string == 'N/A'
 */
long makeLong(string s) {
    if (s == "N/A")
        return (long) -1;

    return atol(s.c_str());

}

/**
 * Converts a string to a float
 * @param s the string to be converted
 * @return float, -1 if the string == 'N/A'
 */
float makeFloat(string s) {
    if (s == "N/A")
        return (float) -1;

    return atof(s.c_str());
}

/**
 * prints all the data members of a Country object
 * @param c the Country to be printed
 * @return void
 */
void printCountry(Country c) {
    cout << "name=" << c.getName() << endl;
    cout << "population=" << c.getPopulation() << endl;
    cout << "literacyRate=" << c.getLiteracyRate() << endl;
    cout << "educationGDPSpent=" << c.getEducationGDPSpent() << endl;
    cout << "primaryCompletionTotal=" << c.getPrimaryCompletionTotal() << endl;
    cout << "primaryCompletionMale=" << c.getPrimaryCompletionMale() << endl;
    cout << "primaryCompletionFemale=" << c.getPrimaryCompletionFemale() << endl;
    cout << "youthLitRateFem=" << c.getYouthLitRateFem() << endl;
    cout << "youthLitRateMale=" << c.getYouthLitRateMale() << endl;
    cout << "________________" << endl;
}

/**
 * lists the countries in each continent
 * @param world the vector of Continents to be listed
 * @return void
 */
void printWorld(vector<Continent> world) {

    for (uint i=0; i<world.size(); i++) {
        vector<Country> temp = world.at(i).getCountriesInContinent();
        cout << "continent '" << world.at(i).getName() << "' -- ";
        cout << temp.size() << " countries" << endl;

        for (uint j=0; j<temp.size(); j++) {
            //printCountry(temp.at(j));
            cout << "   '" << temp.at(j).getName() << "'" << endl;
        }
        cout << "____________________________" << endl;
    }
    cout << endl << endl << "******************************" << endl;
}
