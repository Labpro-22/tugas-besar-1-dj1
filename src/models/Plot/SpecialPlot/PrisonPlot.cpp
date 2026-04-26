#include "models/Plot/SpecialPlot/PrisonPlot.hpp"
#include "views/GameRenderer.hpp"

PrisonPlot::PrisonPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

PlotType PrisonPlot::getType() const {
    return PlotType::PRISONPLOT;
}

void PrisonPlot::startEvent(PlotContext& ctx) {
    Player& currentPlayer = ctx.getCurrentPlayer();
 
    if (currentPlayer.getStatus() == PlayerStatus::JAILED) {
        GameRenderer::showLandOnPrisonJailed(currentPlayer.getJailTurns());
    } else {
        GameRenderer::showLandOnPrisonVisit();
    }
}