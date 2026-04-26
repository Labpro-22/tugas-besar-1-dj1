#include "models/Plot/ActionPlot/LuxuryTaxPlot.hpp"

#include "core/services/BankruptcyService.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

LuxuryTaxPlot::LuxuryTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

void LuxuryTaxPlot::startEvent(PlotContext& ctx) {
    Player& player = ctx.getCurrentPlayer();
    const int taxAmount = PBM;
    const int startingCash = player.getCash();
    BankruptcyService bankruptcyService;

    try {
        player.payTaxes(taxAmount);
        GameRenderer::showLuxuryTax(startingCash, player.getCash());
    } catch (const InsufficientFundException&) {
        GameRenderer::showFailPayTax(player.getCash());
        bankruptcyService.liquidateAssets(player, taxAmount - player.getCash(), ctx.getLogger());
        if (bankruptcyService.canRecover(player, taxAmount)) {
            player.payTaxes(taxAmount);
            GameRenderer::showLuxuryTax(startingCash, player.getCash());
            return;
        }
        bankruptcyService.transferAssets(player, nullptr, ctx.getLogger());
    }
}
