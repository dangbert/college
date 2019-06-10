#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include "Player.h"
#include "CommodityStore.h"
using namespace std;


class Game {
    public:
        //Game(std::stack<Card*> cards, CommodityStore bank, std::vector<Player*> players);
        Game(std::string cards_filename, std::string coms_filename);
        ~Game();
        void runSimulation(int players, Player::STRATEGY strategy);
        void printDrawPile(std::ofstream& fileStream);
        void printResults(std::ofstream& fileStream);


    private:
        void loadDrawPile(std::string filename);

        std::stack<Card*> m_drawPile; // stores the deck of cards
        std::vector<Player*> m_players; // stores the list of players participating
        CommodityStore m_bank; // stores all the commodities in the game

};

#endif
