#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include <queue>
#include "Card.h"
using namespace std;


class Player {
    public:
        enum STRATEGY {
            BEST_PAYOFF,             // 0
            FIRST_ALPHA_DESTINATION, // 1
            COLOR_PURPLE,            // 2
            COLOR_RED,               // 3
            COLOR_ORANGE,            // 4
            COLOR_BLACK,             // 5
            COLOR_GREEN,             // 6
            COLOR_BROWN              // 7
        };

        Player(string name);
        std::string getName() const;
        int getScore() const;
        void addCard(Card *card);
        int calculateScore(Player::STRATEGY strategy);
        void printResults(std::ofstream& fileStream);

    private:
        std::string m_sName; // player’s name to be printed in the output file, format “Player #”
        std::queue<Card*> m_cards; // holds cards in player's hand in order they were dealt to player
        std::queue<Objective*> m_objectives; // holds selected objectives for each card in hand in simulation in order dealt
        int m_score; // players score (0 until calculateScore() is called)

};

#endif
