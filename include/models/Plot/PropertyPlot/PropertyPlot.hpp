#pragma once

#include "models/Plot/Plot.hpp"
#include "models/Plot/PropertyPlot/PropertyStatus.hpp"
#include "models/Player/Player.hpp"

class PropertyPlot: public Plot{
protected:
    int mortgageValue;
    PropertyStatus propertyStatus;
    // Player* ownedBy;
    int festivalDuration;

    bool isFestival() const;

public:
    PropertyPlot(std::string name, std::string code, int mortgageValue, PropertyStatus propertyStatus);
    virtual ~PropertyPlot() = default;

    int getMortgageValue() const;
    PropertyStatus getPropertyStatus() const;

    bool isMortgaged() const;
    void applyFestival();
    virtual int calculateRentPrice() const = 0;
};