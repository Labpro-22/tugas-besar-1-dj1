#pragma once

#include <string>
#include <vector>
#include <map>
#include "models/Plot/Colors.hpp"

class Plot{
protected:
    std::string name;
    std::string code;

public:
    virtual std::string getType() const = 0;
    virtual Color getColor() const = 0;
    void startEvent();
};