#include "models/Plot/ActionPlot/IncomeTaxPlot.hpp"

#include "core/GameException.hpp"
#include "core/services/BankruptcyService.hpp"
#include "core/services/CommandHandler.hpp"
#include "views/GameRenderer.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

IncomeTaxPlot::IncomeTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

int IncomeTaxPlot::getPercentage() const {
    return PPH;
}

void IncomeTaxPlot::startEvent(PlotContext& ctx) {
    GameRenderer::showIncomeTaxPrompt(FLAT, PPH);
    std::string msg = "Pilihan (1/2)"; 
    std::string answer;
    while (true){
        answer = CommandHandler::promptInput("Pilihan (1/2)");
        if (answer == "1" || answer == "2"){
            break;
        }
        else{
            GameRenderer::throwException(InvalidInputException());
        }
    }

    if (answer == "1"){
        if(ctx.getCurrentPlayer().getCash() < FLAT) {
            GameRenderer::showFailPayFlatTax(FLAT, ctx.getCurrentPlayer().getCash());
            // TODO: Handle Bankrupt

        } else {
            ctx.getCurrentPlayer().payTaxes(FLAT);
            GameRenderer::showPayFlatTax(FLAT, ctx.getCurrentPlayer().getCash());
        }
    }

    else if (answer == "2"){
        int totalWealth = ctx.getCurrentPlayer().getTotalWealth();
        int cashBefore = ctx.getCurrentPlayer().getCash();
        ctx.getCurrentPlayer().payTaxes(ctx.getCurrentPlayer().getTotalWealth() * PPH/100);
        GameRenderer::showIncomeTaxResult(totalWealth, cashBefore, PPH);
    }
}
