#include "models/Plot/ActionPlot/ChanceCardPlot.hpp"

#include "models/Board/Board.hpp"
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Player/Player.hpp"
#include "models/Player/PlayerStatus.hpp"

ChanceCardPlot::ChanceCardPlot(std::string name, std::string code, Color color)
    : CardPlot(name, code, color){}

void ChanceCardPlot::startEvent(PlotContext& ctx) {
    std::unique_ptr<ChanceCard> card = ctx.getBoard().drawChanceCard();
    if (!card) {
        return;
    }

    Player& currentPlayer = ctx.getCurrentPlayer();
    const int oldPosition = currentPlayer.getPosition();

    SkillContext skillCtx(ctx);
    card->activate(skillCtx);

    const int newPosition = currentPlayer.getPosition();
    if (newPosition != oldPosition && currentPlayer.getStatus() != PlayerStatus::JAILED) {
        ctx.getBoard().getPlot(newPosition)->startEvent(ctx);
    }
}