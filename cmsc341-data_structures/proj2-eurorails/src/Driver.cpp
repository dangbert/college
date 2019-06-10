#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "Card.h"
#include "Commodity.h"
#include "CommodityStore.h"
#include "Game.h"
#include "Objective.h"
#include "Player.h"

using namespace std;
void printGreeting(std::ofstream& fileStream);

// my driver
// example: ./Driver.out cards.txt commodities.txt 4 3
int main(int argc, char* argv[]) {
    ofstream output;
    output.open("results.txt");
    ofstream& ofs = output;
    printGreeting(ofs);

    if (argc < 5) { // ./driver.out is argv[0]
        ofs << "Error: 5 values must be passed to main. ";
        ofs << "Only " << argc << " were passed." << endl;
        return 0;
    }
    int numPlayers = atoi(argv[3]); // convert to int

    Player::STRATEGY strategy;
    if (atoi(argv[4]) == 0) strategy = Player::BEST_PAYOFF;
    else if (atoi(argv[4]) == 1) strategy = Player::FIRST_ALPHA_DESTINATION;
    else if (atoi(argv[4]) == 2) strategy = Player::COLOR_PURPLE;
    else if (atoi(argv[4]) == 3) strategy = Player::COLOR_RED;
    else if (atoi(argv[4]) == 4) strategy = Player::COLOR_ORANGE;
    else if (atoi(argv[4]) == 5) strategy = Player::COLOR_BLACK;
    else if (atoi(argv[4]) == 6) strategy = Player::COLOR_GREEN;
    else if (atoi(argv[4]) == 7) strategy = Player::COLOR_BROWN;
    else {
        ofs << "Invalid strategy provided" << endl;
        return 0;
    }

    Game game = Game(argv[1], argv[2]); // commodities.txt cards.txt 4 3
    game.printDrawPile(ofs);
    if (numPlayers > 0) {
        game.runSimulation(numPlayers, strategy);
        game.printResults(ofs);
    }
    else {
        ofs << "\n\n Unable to run simulations: there are 0 players." << endl;
    }


    ofs.close();
    return 0;
}


void printGreeting(std::ofstream& fileStream) {
    // need to print to the results file
	fileStream << "Daniel Engbert, CMSC 341 Section 01" << endl;
}
