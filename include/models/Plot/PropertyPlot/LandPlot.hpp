#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class LandPlot: public PropertyPlot{
private:
    Color color;
    int buyPrice;
    int upgHousePrice;
    int upgHotelPrice;
    std::map<int, int> rentPriceTable;
    int level;

public:
    LandPlot(std::string name, std::string code, int mortgageValue,
            Color color, int buyPrice, int upgHousePrice, int upgHotelPrice,
            std::map<int, int> rentPriceTable, PropertyStatus propertyStatus = PropertyStatus::BANK);

    ~LandPlot() = default;

    int getBuyPrice() const;
    int getUpgHousePrice() const;
    int getUpgHotelPrice() const;
    std::map<int, int> getRentPriceTable() const override;
    int getRentPrice(int level) const override;
    int getLevel() const;
    int setLandPlot(Color color, int buyPrice, int upgHousePrice, int upgHotelPrice, std::map<int, int> rentPriceTable);

    void build();
    void sellBuildings();
    bool canBuild() const;
    int calculateRentPrice() const override;
    std::string getType() const override;
    Color getColor() const override;
    void startEvent() override;
};