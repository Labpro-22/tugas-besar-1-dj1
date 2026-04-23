#include "models/Plot/PropertyPlot/StationPlot.hpp"

StationPlot::StationPlot(std::string name, std::string code, Color color, int mortgageValue,
                         Player* owner, PropertyStatus propertyStatus = PropertyStatus::BANK,
                         int festivalDuration = 0, int festivalMultiplier = 1):
    PropertyPlot(name, code, color, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier) {}

std::map<int, int> StationPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int StationPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}

void StationPlot::setRentPriceTable(std::map<int, int> rentPriceTable){
    this->rentPriceTable = rentPriceTable;
}

int StationPlot::calculateRentPrice() const {
    int ownedStation = 0; //TODO: need helper function 
    return rentPriceTable.at(ownedStation)*festivalMultiplier;
}

std::string StationPlot::getType() const {
    return "Petak Stasiun";
}

void StationPlot::startEvent(PlotContext& ctx) {

}