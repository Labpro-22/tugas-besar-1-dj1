#include "models/Card/ChanceCard/BackwardMoveCard.hpp"

#include <sstream>
#include "core/GameException.hpp"
#include "core/SkillContext.hpp"
#include "models/Board/Board.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

const string BackwardMoveCard::getName() const {
    return "BackwardMoveCard";
}

const string BackwardMoveCard::getDescription() const {
    return "Mundur 3 petak";
}

void BackwardMoveCard::activate(SkillContext& ctx) {
    try {
        Player& currPlayer = ctx.getCurrentPlayer();
        int boardSize = ctx.getBoard().getSize();
        currPlayer.move(-3, boardSize);
        
        std::ostringstream oss;
        oss << "Kamu pindah ke " << ctx.getBoard().getPlot(currPlayer.getPosition())->getName();
        GameRenderer::showOnLandChanceCard(*this, oss.str());
    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}
