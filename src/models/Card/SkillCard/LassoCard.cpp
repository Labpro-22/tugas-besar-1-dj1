#include "models/Card/SkillCard/LassoCard.hpp"

void LassoCard::activate(GameState& state) {
    vector<Player>& players = state.getPlayers();
    int currPlayerPosition = state.getCurrentPlayer().getPosition();
    int boardSize = state.getBoard().getSize();

    // Asumsi hanya bisa narik player 1 index setelah current player
    auto it = find_if(players.begin(), players.end(), [&](const Player& p) {
        return (p.getPosition() - currPlayerPosition + boardSize) % boardSize == 1;
    });

    if (it != players.end()) {
        it->moveTo(currPlayerPosition, boardSize);
        it->setUsedSkillThisTurn(true);
    } else {
        throw std::runtime_error("Tidak ada player didepan");
    }
}

const string LassoCard::getName() const{
    return "LassoCard";
}

const string LassoCard::getDescription() const{
    return "Menarik satu pemain lawan yang berada di depan posisi pemain saat ini ke petak tempat pemain tersebut berada.";
}