#include "models/Plot/ActionPlot/IncomeTaxPlot.hpp"

#include <vector>

#include "core/GameException.hpp"
#include "core/PlotContext.hpp"
#include "core/services/BankruptcyService.hpp"
#include "core/services/CommandHandler.hpp"
#include "views/GameRenderer.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

namespace {
bool settleBankTax(PlotContext& ctx, int taxAmount) {
    Player& player = ctx.getCurrentPlayer();

    if (player.isShieldActive() || player.getCash() >= taxAmount) {
        player.payTaxes(taxAmount);
        return true;
    }

    BankruptcyService bankruptcyService;
    bankruptcyService.liquidateAssets(player, taxAmount - player.getCash(), ctx.getLogger());
    if (player.getCash() >= taxAmount) {
        player.payTaxes(taxAmount);
        return true;
    }

    std::vector<Player*> auctionBidders;
    for (Player& bidder : ctx.getPlayers()) {
        auctionBidders.push_back(&bidder);
    }
    bankruptcyService.transferAssets(
        player,
        nullptr,
        ctx.getLogger(),
        auctionBidders,
        ctx.getAuctionService()
    );
    return false;
}
}  // namespace

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
        if(!ctx.getCurrentPlayer().isShieldActive() && ctx.getCurrentPlayer().getCash() < FLAT) {
            GameRenderer::showFailPayFlatTax(FLAT, ctx.getCurrentPlayer().getCash());
        }

        if (settleBankTax(ctx, FLAT)) {
            GameRenderer::showPayFlatTax(FLAT, ctx.getCurrentPlayer().getCash());
        }
    }

    else if (answer == "2"){
        int totalWealth = ctx.getCurrentPlayer().getTotalWealth();
        int cashBefore = ctx.getCurrentPlayer().getCash();
        int taxAmount = totalWealth * PPH / 100;
        if (settleBankTax(ctx, taxAmount)) {
            GameRenderer::showIncomeTaxResult(totalWealth, cashBefore, PPH);
        }
    }
}
