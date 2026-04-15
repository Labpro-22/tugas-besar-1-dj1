#include "models/Plot/PropertyPlot/StationPlot.hpp"

StationPlot::StationPlot(std::string name, std::string code, int mortgageValue,
                        std::map<int, int> rentPriceTable, PropertyStatus propertyStatus = PropertyStatus::BANK):
    PropertyPlot(name, code, mortgageValue, propertyStatus),
    rentPriceTable(rentPriceTable){}

std::map<int, int> StationPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int StationPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}


int StationPlot::calculateRentPrice() const {
    int ownedStation = 0; //TODO: dummy
    return rentPriceTable.at(ownedStation)*festivalMultiplier;
}

std::string StationPlot::getType() const {
    return "Petak Stasiun";
}

Color StationPlot::getColor() const {
    return Color::DEFAULT;
}