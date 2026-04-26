#include "models/Plot/ActionPlot/FestivalPlot.hpp"

#include "core/services/CommandHandler.hpp"
#include "models/Board/Board.hpp"
#include "core/GameException.hpp"
#include "views/GameRenderer.hpp"

FestivalPlot::FestivalPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color) {}

PlotType FestivalPlot::getType() const {
    return PlotType::FESTIVALPLOT;
}

void FestivalPlot::startEvent(PlotContext& ctx) {
    GameRenderer::showFestivalLanding(ctx.getCurrentPlayer());
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
        catch (const GameException e){
           GameRenderer::throwException(e);
        }
        catch (const GameException e){
            GameRenderer::throwException(e);
        }
    }

    ctx.getBoard().getPlots().at(targetIdx).get()->applyFestival();
    // TODO: Handle festival multiplier for Gamerenderer
}