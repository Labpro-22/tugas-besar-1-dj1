#include "models/Plot/ActionPlot/LuxuryTaxPlot.hpp"

#include <vector>

#include "core/PlotContext.hpp"
#include "core/services/BankruptcyService.hpp"
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

LuxuryTaxPlot::LuxuryTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

void LuxuryTaxPlot::startEvent(PlotContext& ctx) {
    if(ctx.getCurrentPlayer().getCash() < PBM) {
        GameRenderer::showFailPayLuxuryTax(ctx.getCurrentPlayer().getCash());
        // TODO: Handle bankrupt

    } else {
        int startMoney = ctx.getCurrentPlayer().getCash();
        int finalMoney = ctx.getCurrentPlayer().getCash() - PBM; 
        GameRenderer::showLuxuryTax(startMoney, finalMoney);
        ctx.getCurrentPlayer().payTaxes(PBM);
    }
}
 