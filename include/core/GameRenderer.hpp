#include "iostream"
#include "core/GameEngine.hpp"
using namespace std;

class GameRenderer {
    public:
        static void showDiceRoll(Player& player, Dice& dice);
        static void showControlDice(Player& player, Dice& dice);
        static void showDeed(Plot& plot);
        static void showPlayerProperties(Player& player);
        static void showBuyProperty();
        static void showRentPayment();
};