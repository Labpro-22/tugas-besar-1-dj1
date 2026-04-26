#include "models/Card/SkillCard/LassoCard.hpp"

#include <algorithm>
#include <sstream>
#include "core/GameException.hpp"
#include "core/SkillContext.hpp"
#include "models/Board/Board.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

void LassoCard::activate(SkillContext& ctx) {
    vector<Player>& players = ctx.getPlayers();
    int currPlayerPosition = ctx.getCurrentPlayer().getPosition();
    int boardSize = ctx.getBoard().getSize();

    Player* nearestPlayer = nullptr;
    int minDistance = boardSize + 1;
    for_each(players.begin(), players.end(), [&](Player& player){
        int distance = (player.getPosition() - currPlayerPosition + boardSize) % boardSize;
        
        if(distance > 0 && distance < minDistance) {
            minDistance = distance;
            nearestPlayer = &player;
        }
    });

    try {
        nearestPlayer->moveTo(currPlayerPosition, boardSize);
        ctx.getCurrentPlayer().setUsedSkillThisTurn(true);

        std::ostringstream oss;
        oss << nearestPlayer->getUsername() << "ditarik ke area kamu!";
        GameRenderer::showActivateSkillCard(LassoCard::getName(), oss.str());
        
    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}

const string LassoCard::getName() const{
    return "LassoCard";
}

const string LassoCard::getDescription() const{
    return "Menarik satu pemain lawan yang berada di depan posisi pemain saat ini ke petak tempat pemain tersebut berada.";
}
