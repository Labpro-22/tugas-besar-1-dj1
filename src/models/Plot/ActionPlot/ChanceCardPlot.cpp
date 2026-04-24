#include "models/Plot/ActionPlot/ChanceCardPlot.hpp"

ChanceCardPlot::ChanceCardPlot(std::string name, std::string code, Color color)
    : CardPlot(name, code, color){}

void ChanceCardPlot::startEvent(PlotContext& ctx) {
    ChanceCard* card = ctx.getBoard().drawChanceCard();
    card->activate(SkillContext(ctx));
}