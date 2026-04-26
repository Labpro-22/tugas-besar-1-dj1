#include "models/Plot/ActionPlot/LuxuryTaxPlot.hpp"

#include "models/Player/Player.hpp"

LuxuryTaxPlot::LuxuryTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

void LuxuryTaxPlot::startEvent(PlotContext& ctx) {
    //TODO: gunakan formatter untuk menampilkan pesan
    ctx.getCurrentPlayer().payTaxes(ctx.getCurrentPlayer().getTotalWealth() * PPH/100);
}