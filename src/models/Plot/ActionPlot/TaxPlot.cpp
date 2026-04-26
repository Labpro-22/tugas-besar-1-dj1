#include "models/Plot/ActionPlot/TaxPlot.hpp"

int TaxPlot::FLAT = 0;
int TaxPlot::PPH = 0;
int TaxPlot::PBM = 0;

TaxPlot::TaxPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color) {}

PlotType TaxPlot::getType() const {
    return PlotType::TAXPLOT;
}

int TaxPlot::getFlatAmount(PlotContext& ctx) const {
    return FLAT;
}

void TaxPlot::setFlatAmount(int amount){
    FLAT = amount;
}

void TaxPlot::setPPH(int amount){
    PPH = amount;
}
void TaxPlot::setPBM(int amount){
    PBM = amount;
}