#include "models/Plot/SpecialPlot/GoPrisonPlot.hpp"
#include "views/GameRenderer.hpp"

GoPrisonPlot::GoPrisonPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

void GoPrisonPlot::startEvent(PlotContext& ctx) {
    GameRenderer::showLandOnGoPrison();
    ctx.getCurrentPlayer().sendToJail(ctx.getBoard());
}