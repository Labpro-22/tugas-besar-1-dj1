#include "models/Plot/PropertyPlot/UtilityPlot.hpp"

#include "models/Board/Dice.hpp"
#include "models/Player/Player.hpp"

std::map<int, int> UtilityPlot::rentPriceTable;

UtilityPlot::UtilityPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue,
                         Player* owner, PropertyStatus propertyStatus,
                         int festivalDuration, int festivalMultiplier):
    PropertyPlot(name, code, color, buyPrice, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier) {}

std::map<int, int> UtilityPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int UtilityPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}

void UtilityPlot::setRentPriceTable(std::map<int, int> rentPriceTable){
    rentPriceTable = rentPriceTable;
}

int UtilityPlot::getLevel() const {
    return owner->countOwnedUtility();
}

int UtilityPlot::calculateRentPrice(PlotContext& ctx) const {
    int ownedUtility = owner->countOwnedUtility();
    int diceTotal = ctx.getDice().getTotal();
    return rentPriceTable.at(ownedUtility)*diceTotal*festivalMultiplier;
}

PlotType UtilityPlot::getType() const {
    return PlotType::UTILITYPLOT;
}