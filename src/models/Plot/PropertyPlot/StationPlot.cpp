#include "models/Plot/PropertyPlot/StationPlot.hpp"

#include <utility>

#include "core/GameException.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

std::map<int, int> StationPlot::rentPriceTable;

StationPlot::StationPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue,
                         Player* owner, PropertyStatus propertyStatus,
                         int festivalDuration, int festivalMultiplier):
    PropertyPlot(name, code, color, buyPrice, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier) {}

std::map<int, int> StationPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int StationPlot::getRentPrice(int level) const {
    auto it = rentPriceTable.find(level);
    if (it == rentPriceTable.end()) {
        return 0;
    }
    return it->second;
}

void StationPlot::setRentPriceTable(std::map<int, int> newRentPriceTable){
    rentPriceTable = std::move(newRentPriceTable);
}

int StationPlot::getLevel() const {
    return owner == nullptr ? 0 : owner->countOwnedStation();
}

int StationPlot::calculateRentPrice(PlotContext& ctx) const {
    return calculateBaseRentPrice(ctx)*festivalMultiplier;
}

int StationPlot::calculateBaseRentPrice(PlotContext& ctx) const {
    (void)ctx;
    int ownedStation = owner == nullptr ? 0 : owner->countOwnedStation();
    auto it = rentPriceTable.find(ownedStation);
    if (it == rentPriceTable.end()) {
        return 0;
    }
    return it->second;
}

PlotType StationPlot::getType() const {
    return PlotType::STATIONPLOT;
}

void StationPlot::startEvent(PlotContext& ctx) {
    Player& currentPlayer = ctx.getCurrentPlayer();

    if (!isOwned()) {
        try {
            currentPlayer.buyProperty(*this, 0);
            GameRenderer::showBuyStation(*this);
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
