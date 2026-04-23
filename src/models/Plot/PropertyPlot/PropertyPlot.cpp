#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

    PropertyPlot::PropertyPlot(std::string name, std::string code, Color color, int mortgageValue, 
                               Player* owner = NULL, PropertyStatus propertyStatus = PropertyStatus::BANK,
                               int festivalDuration = 0, int festivalMultiplier = 1):
        Plot(name, code),
        color(color),
        mortgageValue(mortgageValue),
        propertyStatus(propertyStatus),
        owner(owner),
        festivalDuration(festivalDuration),
        festivalMultiplier(festivalMultiplier)
        {}

    int PropertyPlot::getMortgageValue() const{
        return mortgageValue;
    }

    PropertyStatus PropertyPlot::getPropertyStatus() const{
        return propertyStatus;
    }

    Color PropertyPlot::getColor() const {
        return color;
    }

    Player* PropertyPlot::getOwner() const  {
        return owner;
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