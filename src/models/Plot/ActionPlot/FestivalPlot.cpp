#include "models/Plot/ActionPlot/FestivalPlot.hpp"

FestivalPlot::FestivalPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color) {}

PlotType FestivalPlot::getType() const {
    return PlotType::FESTIVALPLOT;
}

void FestivalPlot::startEvent(PlotContext& ctx) {
    //TODO: tampilkan pesan festival plot
    std::string answer;
    while (true){
        answer = CommandHandler::promptInput("Masukkan nomor petak yang ingin diberi efek festival");
        try{
            int idx = std::stoi(answer);
            if (idx > 0 && idx <= ctx.getBoard().getSize()){
                break;
            }
        }
        catch (std::exception e){}
    }

    int idx = std::stoi(answer);
    ctx.getBoard().getPlots().at(idx).get()->applyFestival(); //TODO perbaiki dengan menambah getPlotByIndex di board 
}