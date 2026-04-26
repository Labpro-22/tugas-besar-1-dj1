#include "models/Plot/PropertyPlot/LandPlot.hpp"

#include "models/Player/Player.hpp"
#include "core/GameException.hpp"
#include "views/GameRenderer.hpp"

LandPlot::LandPlot(std::string name, std::string code, Color color,
            int mortgageValue, int buyPrice, int upgHousePrice, int upgHotelPrice,
            std::map<int, int> rentPriceTable, Player* owner, PropertyStatus propertyStatus,
            int festivalDuration, int festivalMultiplier):
    PropertyPlot(name, code, color, buyPrice, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier),
    upgHousePrice(upgHousePrice), upgHotelPrice(upgHotelPrice),
    rentPriceTable(rentPriceTable),
    level(0){}

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
    auto it = rentPriceTable.find(level);
    if (it == rentPriceTable.end()) {
        return 0;
    }
    return it->second;
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
    level++;
}

void LandPlot::sellBuildings(PlotContext& ctx){
    if (level <= 0){
        throw BuildingIsEmptyException();
    }

    for (auto plot : ctx.getBoard().getPlots(color)){
        if (plot->getLevel() > level){
            throw BuildingLevelInsufficientException();
        }
    }

    level--;
    owner->receive(getSellBuildingPrice());
}

int LandPlot::getSellBuildingPrice() const {
    if (level == 5) return upgHotelPrice;
    else if (level == 0) return 0;
    else return upgHousePrice;
}

bool LandPlot::canBuild(PlotContext& ctx) const {
    if (!isStreetOwned(ctx, &ctx.getCurrentPlayer())){
        throw ColorSetNotOwnedException();
    }
    if (owner->getCash() < getBuildCost()){
        throw InsufficientFundException();
    }
    if (level >= 5){
        throw BuildingIsFullException();
    }
    for (auto plot : ctx.getBoard().getPlots(color)){
        if (plot->getLevel() < level){
            throw BuildingLevelInsufficientException();
        }
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
    int value = isMortgaged() ? 0 : buyPrice;
    value += calculateBuildingValue();
    return value;
}

int LandPlot::calculateRentPrice(PlotContext& ctx) const {
    return calculateBaseRentPrice(ctx)*festivalMultiplier;
}

int LandPlot::calculateBaseRentPrice(PlotContext& ctx) const {
    int rentPrice = getRentPrice(level);

    if (level == 0 && isStreetOwned(ctx, owner)){
        rentPrice *= 2;
    }

    return rentPrice;
}

PlotType LandPlot::getType() const {
    return PlotType::LANDPLOT;
}

void LandPlot::startEvent(PlotContext& ctx) {
    Player& currentPlayer = ctx.getCurrentPlayer();
 
    if (!isOwned()) {
        GameRenderer::showBuyPrompt(currentPlayer, *this);
 
        bool canAfford = currentPlayer.getCash() >= getBuyPrice();
        bool wantsToBuy = canAfford && CommandHandler::promptYesNo("");
 
        if (wantsToBuy) {
            try {
                currentPlayer.buyProperty(*this);
                GameRenderer::showBuySuccess(currentPlayer, *this);
            } catch (const GameException& e) {
                GameRenderer::throwException(e);
            }
        } else {
            GameRenderer::showBuyFailed();
            std::vector<Player*> bidders;
            for (Player& p : ctx.getPlayers()) {
                if (!p.isBankrupt()) {
                    bidders.push_back(&p);
                }
            }
            ctx.getAuctionService().startAuction(*this, bidders, ctx.getLogger());
        }
    } else {
        if (owner != &currentPlayer) {
            if (isMortgaged()) {
                GameRenderer::showMortgagedPlot(*this);
            } else {
                int rentPrice = calculateRentPrice(ctx);
                try {
                    GameRenderer::showPayRent(ctx, *this);
                    currentPlayer.payRent(rentPrice, owner);
                } catch (const InsufficientFundException& e) {
                    GameRenderer::showCannotPayRent(rentPrice, currentPlayer.getCash());
                    GameRenderer::throwException(e);
                }
            }
        }
    }
}

void LandPlot::setLevel(int level) {
    if (level < 0 || level > 5) {
        throw InvalidInputException("Level bangunan harus bernilai antara 0 dan 5.");
    }
    this->level = level;
}
  
void LandPlot::demolish(){
    level = 0;
}
