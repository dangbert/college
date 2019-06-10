#include "Game.h"
#include <fstream>
#include <sstream>
using namespace std;


/*
 * constuctor
 * @param coms_filename: filename of the commodities text file
 * @param cards_filename: filename of the cards text file
 */
Game::Game(std::string cards_filename, std::string coms_filename) {
    // TODO: check these files exist
    m_bank.loadCommodities(coms_filename); // initiate m_bank
    loadDrawPile(cards_filename); // initiate m_drawPile
}

/*
 * Destructor
 */
Game::~Game() {
    // deallocate cards
    while (!m_drawPile.empty()) {
        delete m_drawPile.top();
        m_drawPile.pop();
    }

    for (uint i=0; i<m_players.size(); i++) {
        delete m_players.at(i);
    }
}

/*
 * runs through the simulation of playing the game
 *
 * create player objects
 * deals cards, making sure everyone gets the same amount (some might be unused)
 *
 * @param players: number of players for the simulation
 * @param stategy: strategy to use for the simulation
 */
void Game::runSimulation(int players, Player::STRATEGY strategy) {
    // inititate m_players
    std::ostringstream ss;
    for (int i=1; i<=players; i++) {
        ss.str(""); // clear ss
        ss << "Player " << i;
        Player* p = new Player(ss.str()); // example name: "Player 3"
        m_players.push_back(p); // store player pointer in vector
    }

    // deal out the cards to the players
    std::stack<Card*> temp = m_drawPile; // copy of the draw pile

    // traverse all of m_players as many times as needed
    for (uint i=0; i<m_drawPile.size()/players; i++) {

        // cycle through all players
        for (uint p=0; p<m_players.size(); p++) {
            // add card at top of stack to current player
            m_players.at(p)->addCard(temp.top());
            temp.pop();
        }
    }

    // calculate each player's score
    for (uint p=0; p<m_players.size(); p++) {
        m_players.at(p)->calculateScore(strategy);
    }
}

/*
 * prints the current state of the drawPile to a given output stream
 */
void Game::printDrawPile(std::ofstream& fileStream) {
    std::stack<Card*> temp = m_drawPile; // needed to traverse stack without emptying it
    fileStream << "---------- Draw Pile ----------" << endl;

    while (!temp.empty()) {
        temp.top()->printCard(fileStream);
        temp.pop();
    }

}

/*
 * prints the result of the simulation to a given output stream
 */
void Game::printResults(std::ofstream& fileStream) {
    fileStream << "\n\n---------- RESULTS ----------" << endl;
    Player* winner = m_players.at(0);
    for (uint p=0; p<m_players.size(); p++) {
        m_players.at(p)->printResults(fileStream);
        if (m_players.at(p)->getScore() > winner->getScore()) {
            winner = m_players.at(p);
        }
    }
    fileStream << "--------------------------" << endl;
    fileStream << "--------------------------" << endl;
    fileStream << "Winner: " << winner->getName();
    fileStream << " Score: " << winner->getScore() << endl;
}

/*
 * loads the deck from the provided file
 * initiates m_drawPile (assume m_bank is already initialized)
 */
void Game::loadDrawPile(std::string filename) {
    ifstream ifile(filename.c_str());
    string destination[3]; // destinations on one card
    string com_string[3]; // comodities
    string pay_string[3]; // payoffs

    while (!ifile.eof()) {
        // loop through the 3 objectives on the card
        for (int i=0; i<3; i++) {
            ifile >> destination[i]; // store next string in file in destination
            ifile >> com_string[i];
            ifile >> pay_string[i];
            if (ifile.eof()) { // check that we haven't reached the end of the file
                break;
            }
            if (i == 2) { // if 3 objectives were found for this card
                Card* c = new Card();
                // create and store each objective for the card
                for (int k=0; k<3; k++) {
                    int payoff = atoi(pay_string[k].c_str()); // convert to int
                    // find Commodity* in commodity store
                    Commodity* commodity = m_bank.getCommodity(com_string[k]);

                    // add objectives to card
                    Objective* o = new Objective(destination[k], commodity, payoff);
                    c->addObjective(o); // add objective to this card
                }
                m_drawPile.push(c); // push card pointer into stack
            }
        }

    }
}
