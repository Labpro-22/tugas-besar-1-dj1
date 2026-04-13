#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class StationPlot: public PropertyPlot{
private:
    std::map<int, int> rentPrice;

public:
    StationPlot();
    ~StationPlot() = default;

    std::map<int, int> getRentPrice() const;

    int calculateRentPrice() const override;
    std::string getType() const override;
    Color getColor() const override;
};