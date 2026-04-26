#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
#include "core/services/AuctionService.hpp"
#include "models/Board/Board.hpp"
#include "core/services/CommandHandler.hpp"

class LandPlot: public PropertyPlot{
private:
    int upgHousePrice;
    int upgHotelPrice;
    std::map<int, int> rentPriceTable;
    int level;

public:
    LandPlot(std::string name, std::string code, Color color,
            int buyPrice, int mortgageValue, int upgHousePrice, int upgHotelPrice,
            std::map<int, int> rentPriceTable, Player* owner = NULL,
            PropertyStatus propertyStatus = PropertyStatus::BANK,
            int festivalDuration = 0, int festivalMultiplier = 1);

    ~LandPlot() = default;

    int getUpgHousePrice() const;
    int getUpgHotelPrice() const;
    std::map<int, int> getRentPriceTable() const override;
    int getRentPrice(int level) const override;
    void setLevel(int level);
    int getLevel() const override;

    void build(PlotContext& ctx);
    void sellBuildings();
    int getSellBuildingPrice() const;
    bool canBuild(PlotContext& ctx) const;
    int getBuildCost() const;
    bool isStreetOwned(PlotContext& ctx, Player* player) const;

    std::string getBuildingType() const;
    int getBuildingCount() const;
    int calculateBuildingValue() const;
    int calculateTotalValue() const override;
    int calculateRentPrice(PlotContext& ctx) const override;

    PlotType getType() const override;
    void startEvent(PlotContext& ctx) override;
    
    void demolish() override;
};