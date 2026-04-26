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
    int targetIdx;
    while (true){
        std::string answer = CommandHandler::promptInput("Masukkan nomor petak yang ingin diberi efek festival");
        try{
            int idx = std::stoi(answer);
            if (!ctx.getBoard().isPropertyPlot(idx)){
                throw InvalidPlotTypeException();
            }
            targetIdx = idx;
            break;
        }
        catch (GameException e){
            // std::cout << e.what(); //TODO: tampilkan dengan renderer
        }
        catch (std::exception e){
            // std::cout << InvalidInputException().what(); //TODO: tampilkan dengan renderer
        }
    }

    ctx.getBoard().getPlots().at(targetIdx).get()->applyFestival();
}