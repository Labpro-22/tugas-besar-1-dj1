#include "models/Plot/PropertyPlot/StationPlot.hpp"

#include "models/Player/Player.hpp"

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