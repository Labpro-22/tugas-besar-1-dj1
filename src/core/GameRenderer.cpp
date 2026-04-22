#include "core/GameRenderer.hpp"

void GameRenderer::showDiceRoll(Player& player, Dice& dice) {
    cout << "Mengocok dadu..." << endl;
    showControlDice(player, dice);
};

void GameRenderer::showControlDice(Player& player, Dice& dice) {
    player.move(dice.getTotal());
    cout << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    cout << "Memajukan Bidak " << player.getUsername() << " sebanyak " << dice.getTotal() << "petak..." << endl;
    cout << player.getUsername() << " mendarat di " << player.getPosition() << endl;
};
void GameRenderer::showDeed(Plot& plot) {
    cout << "+===========================================+" << endl;
    cout << "|              AKTA KEPEMILIKAN             |" << endl;
    cout << "+===========================================+" << endl;

};    
void GameRenderer::showPlayerProperties(Player& player) {

};