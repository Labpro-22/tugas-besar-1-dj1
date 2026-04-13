#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class LandPlot: public PropertyPlot{
private:
    Color color;
    int buyPrice;
    int upgHousePrice;
    int upgHotelPrice;
    std::vector<int> rentPricePerLevel;
    int level;

public:
    LandPlot();
    ~LandPlot() = default;

    int getBuyPrice();
    int getUpgHousePrice();
    int getUpgHotelPrice();
    int getRentPrice(int level) const;
    int getLevel();

    void build();
    void sellBuildings();
    bool canBuild() const;
    int calculateRentPrice() const override;
    std::string getType() const override;
    Color getColor() const override;
};