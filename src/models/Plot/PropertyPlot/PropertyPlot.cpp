#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

PropertyPlot::PropertyPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue,
                            Player* owner, PropertyStatus propertyStatus,
                            int festivalDuration, int festivalMultiplier):
    Plot(name, code, color),
    buyPrice(buyPrice),
    mortgageValue(mortgageValue),
    propertyStatus(propertyStatus),
    owner(owner),
    festivalDuration(festivalDuration),
    festivalMultiplier(festivalMultiplier)
    {}

int PropertyPlot::getBuyPrice() const{
    return buyPrice;
}

int PropertyPlot::getMortgageValue() const{
    return mortgageValue;
}

int PropertyPlot::getFestivalDuration() const {
    return festivalDuration;
}

int PropertyPlot::getFestivalMultiplier() const {
    return festivalMultiplier;
}

int PropertyPlot::getSellPrice() const {
    return buyPrice;
}

PropertyStatus PropertyPlot::getPropertyStatus() const{
    return propertyStatus;
}

Player* PropertyPlot::getOwner() const  {
    return owner;
}

void PropertyPlot::setOwner(Player* player){
    owner = player;
}

bool PropertyPlot::isOwned() const{
    return owner != NULL;
}

bool PropertyPlot::isMortgaged() const{
    return propertyStatus == PropertyStatus::MORTGAGED;
}
void PropertyPlot::applyFestival(){
    if (festivalMultiplier < 8){
        festivalMultiplier *= 2;
    }
    festivalDuration = 3;
}

void PropertyPlot::updateFestival(){
    if (festivalDuration == 0) return;
    else festivalDuration--;
    if (festivalDuration == 0) endFestival();
}

void PropertyPlot::endFestival(){
    festivalDuration = 0;
    festivalMultiplier = 1;
}

int PropertyPlot::calculateBuildingValue() const{
    return 0;
}

int PropertyPlot::calculateTotalValue() const{
    return buyPrice;
}

void PropertyPlot::setFestivalMultiplier(int mult) {
    if (mult < 1 || mult > 8) {
        throw InvalidInputException("Festival multiplier harus bernilai antara 1 dan 8.");
    }
    festivalMultiplier = mult;
}
 
void PropertyPlot::setFestivalDuration(int dur) {
    if (dur < 0) {
        throw InvalidInputException("Festival duration tidak boleh negatif.");
    }
    festivalDuration = dur;
}
 
void PropertyPlot::setPropertyStatus(PropertyStatus status) {
    propertyStatus = status;
}