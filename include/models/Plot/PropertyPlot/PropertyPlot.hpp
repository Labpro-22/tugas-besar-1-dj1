#pragma once

#include "models/Plot/Plot.hpp"
#include "models/Plot/PropertyPlot/PropertyStatus.hpp"
#include "models/Player/Player.hpp"

class PropertyPlot: public Plot{
protected:
    int mortgageValue;
    PropertyStatus PropertyStatus;
    // Player* ownedBy;
    int festivalDuration;

    bool isFestival() const;

public:
    bool isMortgaged() const;
    void applyFestival();
    virtual int calculateRentPrice() const = 0;
    virtual void startEvent();
};