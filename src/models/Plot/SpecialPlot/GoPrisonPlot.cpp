#include "models/Plot/SpecialPlot/GoPrisonPlot.hpp"

GoPrisonPlot::GoPrisonPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

void GoPrisonPlot::startEvent(PlotContext& ctx) {
    //TODO: Tampilkan message
    ctx.getCurrentPlayer().sendToJail(ctx.getBoard());
}