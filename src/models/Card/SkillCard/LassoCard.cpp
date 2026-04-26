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
    Player& currentPlayer = ctx.getCurrentPlayer();
    int currPlayerPosition = currentPlayer.getPosition();
    int boardSize = ctx.getBoard().getSize();

    Player* nearestPlayer = nullptr;
    int minDistance = boardSize + 1;
    for_each(players.begin(), players.end(), [&](Player& player){
        if (&player == &currentPlayer || player.isBankrupt()) {
            return;
        }

        int distance = (player.getPosition() - currPlayerPosition + boardSize) % boardSize;

        if(distance > 0 && distance < minDistance) {
            minDistance = distance;
            nearestPlayer = &player;
        }
    });

    try {
        if (!nearestPlayer) {
            GameRenderer::showActivateSkillCard(
                LassoCard::getName(),
                "Tidak ada pemain lawan di depan posisi kamu."
            );
            return;
        }

        nearestPlayer->moveTo(currPlayerPosition, boardSize);
        currentPlayer.setUsedSkillThisTurn(true);

        std::ostringstream oss;
        oss << nearestPlayer->getUsername() << " ditarik ke area kamu!";
        GameRenderer::showActivateSkillCard(LassoCard::getName(), oss.str());

    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}

const string LassoCard::getName() const{
    return "LassoCard";
}

const string LassoCard::getDescription() const{
    return "Menarik satu pemain lawan yang berada di depan posisi pemain saat ini ke petak tempat pemain saat ini berada.";
}
