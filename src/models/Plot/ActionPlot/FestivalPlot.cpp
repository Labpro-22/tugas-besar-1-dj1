#include "models/Plot/ActionPlot/FestivalPlot.hpp"

#include "core/services/CommandHandler.hpp"
#include "models/Board/Board.hpp"

FestivalPlot::FestivalPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color) {}

PlotType FestivalPlot::getType() const {
    return PlotType::FESTIVALPLOT;
}

void FestivalPlot::startEvent(PlotContext& ctx) {
    //TODO: tampilkan pesan festival plot
    std::string answer = CommandHandler::promptInput("Masukkan nomor petak yang ingin diberi efek festival"); //TODO: perbaiki prompt input dengan lambda function untuk validasi input
    //Note: asumsi answer sudah valid
    int idx = std::stoi(answer);
    ctx.getBoard().getPlots().at(idx).get()->applyFestival(); //TODO perbaiki dengan menambah getPlotByIndex di board 
}