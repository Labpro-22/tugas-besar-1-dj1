#pragma once
#include <string>
#include "models/Board/CardDeck.hpp"
using namespace std;

class CommunityChestCard {
    public:
        virtual void activate() = 0;
        virtual string getName() = 0;
        virtual string getDescription() = 0;
};
 