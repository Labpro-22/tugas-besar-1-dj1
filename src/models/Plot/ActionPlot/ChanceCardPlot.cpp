#include "models/Plot/ActionPlot/ChanceCardPlot.hpp"

#include "models/Board/Board.hpp"
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Player/Player.hpp"
#include "models/Player/PlayerStatus.hpp"

ChanceCardPlot::ChanceCardPlot(std::string name, std::string code, Color color)
    : CardPlot(name, code, color){}

void ChanceCardPlot::startEvent(PlotContext& ctx) {
    Board& board = ctx.getBoard();
    std::unique_ptr<ChanceCard> card = board.drawChanceCard();

    SkillContext skillCtx(ctx);
    card->activate(skillCtx);

    //Kembalikan kartu
    board.getChanceDeckPile().discard(std::move(card));
    board.getChanceDeckPile().reshuffleFromDiscard();

    //Aktifkan efek land di plot yang baru
    board.getPlot(ctx.getCurrentPlayer().getPosition())->startEvent(ctx);
}