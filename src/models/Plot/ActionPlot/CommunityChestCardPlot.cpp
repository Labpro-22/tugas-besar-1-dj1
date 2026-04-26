#include "models/Plot/ActionPlot/CommunityChestCardPlot.hpp"

#include "models/Board/Board.hpp"
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"

CommunityChestCardPlot::CommunityChestCardPlot(std::string name, std::string code, Color color)
    : CardPlot(name, code, color) {}

void CommunityChestCardPlot::startEvent(PlotContext& ctx) {
    Board& board = ctx.getBoard();
    std::unique_ptr<CommunityChestCard> card = ctx.getBoard().drawCommunityChestCard();
    SkillContext skillCtx(ctx);
    if (card) {
        card->activate(skillCtx);
    }

    //Kembalikan kartu
    board.getCommunityChestDeckPile().discard(std::move(card));
    board.getCommunityChestDeckPile().reshuffleFromDiscard();
}