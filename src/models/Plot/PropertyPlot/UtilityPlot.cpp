#include "models/Plot/PropertyPlot/UtilityPlot.hpp"

#include <utility>

#include "core/GameException.hpp"
#include "models/Board/Dice.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

std::map<int, int> UtilityPlot::rentPriceTable;

UtilityPlot::UtilityPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue,
                         Player* owner, PropertyStatus propertyStatus,
                         int festivalDuration, int festivalMultiplier):
    PropertyPlot(name, code, color, buyPrice, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier) {}

std::map<int, int> UtilityPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int UtilityPlot::getRentPrice(int level) const {
    auto it = rentPriceTable.find(level);
    if (it == rentPriceTable.end()) {
        return 0;
    }
    return it->second;
}

void UtilityPlot::setRentPriceTable(std::map<int, int> newRentPriceTable){
    rentPriceTable = std::move(newRentPriceTable);
}

int UtilityPlot::getLevel() const {
    return owner == nullptr ? 0 : owner->countOwnedUtility();
}

int UtilityPlot::calculateRentPrice(PlotContext& ctx) const {
    return calculateBaseRentPrice(ctx)*festivalMultiplier;
}

int UtilityPlot::calculateBaseRentPrice(PlotContext& ctx) const {
    int ownedUtility = owner == nullptr ? 0 : owner->countOwnedUtility();
    int diceTotal = ctx.getDice().getTotal();
    auto it = rentPriceTable.find(ownedUtility);
    if (it == rentPriceTable.end()) {
        return 0;
    }
    return it->second*diceTotal;
}

PlotType UtilityPlot::getType() const {
    return PlotType::UTILITYPLOT;
}

void UtilityPlot::startEvent(PlotContext& ctx) {
    Player& currentPlayer = ctx.getCurrentPlayer();
 
    if (!isOwned()) {
        try {
            currentPlayer.buyProperty(*this, 0);
            GameRenderer::showBuyUtility(*this);
        } catch (const GameException& e) {
            GameRenderer::throwException(e);
        }
    } else {
        if (owner != &currentPlayer) {
            if (isMortgaged()) {
                GameRenderer::showMortgagedPlot(*this);
            } else {
                int rentPrice = calculateRentPrice(ctx);
                try {
                    GameRenderer::showPayRent(ctx, *this);
                    currentPlayer.payRent(rentPrice, owner);
                } catch (const InsufficientFundException& e) {
                    GameRenderer::showCannotPayRent(rentPrice, currentPlayer.getCash());
                    GameRenderer::throwException(e);
                }
            }
        }
    }
}
