#include "models/Plot/ActionPlot/LuxuryTaxPlot.hpp"

LuxuryTaxPlot::LuxuryTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

void LuxuryTaxPlot::startEvent(PlotContext& ctx) {
    //TODO: gunakan formatter untuk menampilkan pesan
    ctx.getCurrentPlayer().pay(ctx.getCurrentPlayer().getTotalWealth() * PPH/100);//TODO: gunakan payTaxes atau tangani bankrupt
}