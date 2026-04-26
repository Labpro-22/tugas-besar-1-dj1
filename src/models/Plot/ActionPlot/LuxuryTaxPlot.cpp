#include "models/Plot/ActionPlot/LuxuryTaxPlot.hpp"

#include "core/services/BankruptcyService.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

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
        ctx.getCurrentPlayer().payTaxes(ctx.getCurrentPlayer().getTotalWealth() * PBM/100);
    }
}
