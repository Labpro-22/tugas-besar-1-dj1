#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class StationPlot: public PropertyPlot{
private:
    std::map<int, int> rentPriceTable;

public:
    StationPlot(std::string name, std::string code, int mortgageValue,
            std::map<int, int> rentPriceTable, PropertyStatus propertyStatus = PropertyStatus::BANK);
    ~StationPlot() = default;

    std::map<int, int> getRentPriceTable() const override;
    int getRentPrice(int level) const override;

    int calculateRentPrice() const override;
    std::string getType() const override;
    Color getColor() const override;
    void startEvent() override;
};