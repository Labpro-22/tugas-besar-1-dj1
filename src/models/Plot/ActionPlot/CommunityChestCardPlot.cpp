#include "models/Plot/ActionPlot/CommunityChestCardPlot.hpp"

CommunityChestCardPlot::CommunityChestCardPlot(std::string name, std::string code, Color color)
    : CardPlot(name, code, color) {}

void CommunityChestCardPlot::startEvent(PlotContext& ctx) {
    CommunityChestCard* card = ctx.getBoard().drawCommunityChestCard();
    card->activate(SkillContext(ctx));
}