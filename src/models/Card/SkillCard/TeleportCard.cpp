#include "models/Card/SkillCard/TeleportCard.hpp"
#include "core/GameException.hpp"

void TeleportCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    int boardSize = ctx.getBoard().getSize();

    try {
        GameRenderer::showActivateSkillCard(TeleportCard::getName(), "Kamu dapat TELEPORT ke plot mana saja!");
        string index = CommandHandler::promptInput("Pilih index plot untuk TELEPORT");
        currPlayer.moveTo(stoi(index), boardSize);
        currPlayer.setUsedSkillThisTurn(true);
    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}

const string TeleportCard::getName() const{
    return "TeleportCard";
}

const string TeleportCard::getDescription() const {
    return "Memberikan pemain kebebasan untuk berpindah ke petak manapun di atas papan permainan.";
}
