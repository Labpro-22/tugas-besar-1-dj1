#include "models/Plot/PropertyPlot/UtilityPlot.hpp"

UtilityPlot::UtilityPlot(std::string name, std::string code, int mortgageValue,
                        std::map<int, int> rentPriceTable, PropertyStatus propertyStatus = PropertyStatus::BANK):
    PropertyPlot(name, code, mortgageValue, propertyStatus),
    rentPriceTable(rentPriceTable){}

std::map<int, int> UtilityPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int UtilityPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}

int UtilityPlot::calculateRentPrice() const {
    int ownedUtility = 0; //TODO: dummy
    int diceTotal = 6; //TODO: dummy
    return rentPriceTable.at(ownedUtility)*diceTotal*festivalMultiplier;
}

std::string UtilityPlot::getType() const {
    return "Petak Utilitas";
}

Color UtilityPlot::getColor() const {
    return Color::GRAY;
}
