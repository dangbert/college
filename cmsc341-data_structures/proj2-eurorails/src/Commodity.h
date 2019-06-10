#ifndef _COMMODITY_H_
#define _COMMODITY_H_

#include <iostream>
using namespace std;


class Commodity {
    public:
        enum COLOR {
            PURPLE,   // 0
            RED,      // 1
            ORANGE,   // 2
            BLACK,    // 3
            GREEN,    // 4
            BROWN,    // 5
            COLORLESS // 6
        };

        Commodity(string name, COLOR color);
        std::string getName() const;
        COLOR getColor();


    private:
        std::string m_sName; // string name of the commodity
        COLOR m_eColor; // the Color enum index of the commodity's color

};

#endif
