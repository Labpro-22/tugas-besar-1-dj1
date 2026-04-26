#include "models/Plot/SpecialPlot/FreeParkPlot.hpp"
#include <iostream>

FreeParkPlot::FreeParkPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

void FreeParkPlot::startEvent(PlotContext& ctx) {
    //TODO: tampilkan pesan (udah bener kok ini)
}