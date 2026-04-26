#include "models/Plot/PropertyPlot/StationPlot.hpp"

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
    return rentPriceTable.at(level);
}

void StationPlot::setRentPriceTable(std::map<int, int> rentPriceTable){
    rentPriceTable = rentPriceTable;
}

int StationPlot::getLevel() const {
    return owner->countOwnedStation();
}

int StationPlot::calculateRentPrice(PlotContext& ctx) const {
    int ownedStation = owner->countOwnedStation();
    return rentPriceTable.at(ownedStation)*festivalMultiplier;
}

PlotType StationPlot::getType() const {
    return PlotType::STATIONPLOT;
}

void StationPlot::startEvent(PlotContext& ctx) {
    Player& currentPlayer = ctx.getCurrentPlayer();

    if (!isOwned()) {
        try {
            currentPlayer.buyProperty(*this);
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