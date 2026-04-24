#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

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

void PropertyPlot::endFestival(){
    festivalDuration = 0;
    festivalMultiplier = 1;
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