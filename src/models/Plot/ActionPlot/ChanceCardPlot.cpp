#include "models/Plot/ActionPlot/ChanceCardPlot.hpp"

#include "models/Board/Board.hpp"
#include "models/Card/ChanceCard/ChanceCard.hpp"

ChanceCardPlot::ChanceCardPlot(std::string name, std::string code, Color color)
    : CardPlot(name, code, color){}

void ChanceCardPlot::startEvent(PlotContext& ctx) {
    std::unique_ptr<ChanceCard> card = ctx.getBoard().drawChanceCard();
    SkillContext skillCtx(ctx);
    if (card) {
        card->activate(skillCtx);
    }
}