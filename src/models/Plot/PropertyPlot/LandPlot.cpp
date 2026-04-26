#include "models/Plot/PropertyPlot/LandPlot.hpp"

#include "core/services/CommandHandler.hpp"
#include "models/Player/Player.hpp"

LandPlot::LandPlot(std::string name, std::string code, Color color,
            int mortgageValue, int buyPrice, int upgHousePrice, int upgHotelPrice,
            std::map<int, int> rentPriceTable, Player* owner, PropertyStatus propertyStatus,
            int festivalDuration, int festivalMultiplier):
    PropertyPlot(name, code, color, buyPrice, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier),
    upgHousePrice(upgHousePrice), upgHotelPrice(upgHotelPrice),
    rentPriceTable(rentPriceTable){}

int LandPlot::getUpgHousePrice() const {
    return upgHotelPrice;
}

int LandPlot::getUpgHotelPrice() const {
    return upgHotelPrice;
}

std::map<int, int> LandPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int LandPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}

int LandPlot::getLevel() const {
    return level;
}

void LandPlot::build(PlotContext& ctx){
    canBuild(ctx);
    
    if (level == 4){
        owner->pay(upgHotelPrice);
    }
    else{
        owner->pay(upgHousePrice);
    }
}

void LandPlot::sellBuildings(){
    if (level <= 0){
        throw BuildingIsEmptyException();
    }

    level--;
    owner->receive(getSellBuildingPrice());
}

void LandPlot::destroyBuilding(){
    level = 0;
}

void LandPlot::downgradeBuilding(){
    if (level > 0) level--;
}

int LandPlot::getSellBuildingPrice() const {
    if (level == 5) return upgHotelPrice;
    else if (level == 0) return 0;
    else return upgHousePrice;
}

bool LandPlot::canBuild(PlotContext& ctx) const {
    int buildCost = getBuildCost();
    if (!isStreetOwned(ctx, &ctx.getCurrentPlayer())){
        throw ColorSetNotOwnedException();
    }
    if (owner->getCash() < getBuildCost()){
        throw InsufficientFundException();
    }
    if (level >= 5){
        throw BuildingIsFullException();


    }
    return true;
} 

int LandPlot::getBuildCost() const {
    if (level < 4){
        return upgHousePrice;
    }
    else if (level == 4){
        return upgHotelPrice;
    } else {
        return 0;
    }
}

bool LandPlot::isStreetOwned(PlotContext& ctx, Player* player) const {
    if (owner == NULL) return false;
    return ctx.getBoard().isPlayerOwnAllColor(color, player);
}

std::string LandPlot::getBuildingType() const {
    if (level == 5) return "hotel";
    else if (level == 0) return "kosong";
    else return "rumah";
}

int LandPlot::getBuildingCount() const{
    if (level == 5) return 1;
    else return level;
}

int LandPlot::calculateBuildingValue() const{
    int value = 0;
    if (level == 5){
        value += upgHotelPrice;
    }
    value += upgHousePrice * std::min(level, 4);
    return value;
}

int LandPlot::calculateTotalValue() const{
    int value = isMortgaged() ? 0 : buyPrice; //TODO cek bagaimana harga dihitung atau tidak jika mortgaged
    value += calculateBuildingValue();
    return value;
}

int LandPlot::calculateRentPrice(PlotContext& ctx) const {
    int rentPrice = rentPriceTable.at(level);

    if (level == 0 && isStreetOwned(ctx, owner)){
        rentPrice *= 2;
    }

    return rentPrice*festivalMultiplier;
}

PlotType LandPlot::getType() const {
    return PlotType::LANDPLOT;
}

void LandPlot::startEvent(PlotContext& ctx){
    if (!isOwned()){
        if (ctx.getCurrentPlayer().getCash() >= getBuyPrice()
            && CommandHandler::promptYesNo("Apakah anda ingin membeli petak ini (harga: M" + std::to_string(getBuyPrice()) + ")?")){
            ctx.getCurrentPlayer().buyProperty(*this);
        }
        else{
            // ctx.getAuctionService().startAuction(); //TODO: perbaiki auction service
        }
    }
    else{
        if (owner != &ctx.getCurrentPlayer()){
            int rentPrice = calculateRentPrice(ctx);
            ctx.getCurrentPlayer().payRent(rentPrice, owner);
        }
    }
}

void LandPlot::setLevel(int level) {
    if (level < 0 || level > 5) {
        throw InvalidInputException("Level bangunan harus bernilai antara 0 dan 5.");
    }
    this->level = level;
}