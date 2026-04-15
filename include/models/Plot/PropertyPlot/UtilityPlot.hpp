#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class UtilityPlot: public PropertyPlot{
private:
    std::map<int, int> rentPriceTable;

public:
    UtilityPlot(std::string name, std::string code, int mortgageValue,
            std::map<int, int> rentPriceTable, PropertyStatus propertyStatus = PropertyStatus::BANK);
    ~UtilityPlot() = default;
    
    std::map<int, int> getRentPriceTable() const override;
    int getRentPrice(int level) const override;

    int calculateRentPrice() const override;
    std::string getType() const override;
    Color getColor() const override;
    void startEvent() override;
};