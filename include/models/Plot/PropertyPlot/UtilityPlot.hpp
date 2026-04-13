#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class UtilityPlot: public PropertyPlot{
private:
    std::map<int, int> rentPrice;
    
public:
    int calculateRentPrice() const override;

    std::string getType() const override;

    Color getColor() const override;
};