#include "Player.h"
using namespace std;


/*
 * constructor
 */
Player::Player(string name)
    : m_sName(name), m_score(0)
{

}

/*
 * returns the string name of the player (format "Player #")
 */
std::string Player::getName() const {
    return m_sName;
}

/*
 * returns the score received by the plaer after running the simulation
 * default 0
 */
int Player::getScore() const {
    return m_score;
}

/*
 * adds a card to the players hand
 * @param card: pointer to the Card to be added
 */
void Player::addCard(Card* card) {
    m_cards.push(card);
}

/*
 * resets the selected objective then processes the players hand using
 * the given strategy.
 *
 * stores resulting objectives selected in m_objectives
 * stores resulting score in m_score
 * can be run on a player's hand multiple times
 *
 * @param strategy: the strategy to use
 * @return score of the player
 */
int Player::calculateScore(Player::STRATEGY strategy) {
    m_score = 0;
    // empty m_objectives
    while (!m_objectives.empty()) {
        m_objectives.pop();
    }

    std::queue<Card*> hand = m_cards; // copy of m_cards
    while (!hand.empty()) {
        Objective* selected; // the objective from this card to complete
        if (strategy == Player::BEST_PAYOFF) {
            selected = hand.front()->getBestPayoff();
        }
        else if (strategy == Player::FIRST_ALPHA_DESTINATION) {
            selected = hand.front()->getFirstAlphaDestination();
        }
        else { // strategy is a color
            Commodity::COLOR color = Commodity::COLORLESS;
            if (strategy == Player::COLOR_PURPLE) color = Commodity::PURPLE;
            if (strategy == Player::COLOR_RED) color = Commodity::RED;
            if (strategy == Player::COLOR_ORANGE) color = Commodity::ORANGE;
            if (strategy == Player::COLOR_BLACK) color = Commodity::BLACK;
            if (strategy == Player::COLOR_GREEN) color = Commodity::GREEN;
            if (strategy == Player::COLOR_BROWN) color = Commodity::BROWN;

            selected = hand.front()->getByColor(color);
        }
        m_objectives.push(selected); // store resulting objective
        m_score += selected->getPayoff();
        hand.pop();
    }
    return m_score;
}

/*
 * prints the resulting objectives selected by the player from
 * each card and the player’s final score
 */
void Player::printResults(std::ofstream& fileStream) {
    fileStream << "--------" << getName() << "------------" << endl;
    // print each completed objective
    while (!m_objectives.empty()) {
        m_objectives.front()->printObjective(fileStream);
        m_objectives.pop();
    }
    fileStream << "Score: " << m_score << endl;
}
