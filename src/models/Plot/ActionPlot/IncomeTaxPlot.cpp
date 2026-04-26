#include "models/Plot/ActionPlot/IncomeTaxPlot.hpp"

#include "core/GameException.hpp"
#include "core/services/BankruptcyService.hpp"
#include "core/services/CommandHandler.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

IncomeTaxPlot::IncomeTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

int IncomeTaxPlot::getPercentage() const {
    return percentage;
}

void IncomeTaxPlot::startEvent(PlotContext& ctx) {
    GameRenderer::showIncomeTaxPrompt(FLAT, PPH);
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

    Player& player = ctx.getCurrentPlayer();
    const int taxAmount = (answer == "1")
        ? FLAT
        : player.getTotalWealth() * PPH / 100;

    if (answer == "2") {
        GameRenderer::showIncomeTaxResult(player.getTotalWealth(), player.getCash(), PPH);
    }

    BankruptcyService bankruptcyService;
    try {
        player.payTaxes(taxAmount);
    } catch (const InsufficientFundException&) {
        GameRenderer::showFailPayTax(player.getCash());
        bankruptcyService.liquidateAssets(player, taxAmount - player.getCash(), ctx.getLogger());
        if (bankruptcyService.canRecover(player, taxAmount)) {
            player.payTaxes(taxAmount);
            return;
        }
        bankruptcyService.transferAssets(player, nullptr, ctx.getLogger());
    }
}
