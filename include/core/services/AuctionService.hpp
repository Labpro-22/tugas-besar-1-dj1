#pragma once

#include <vector>

class PropertyPlot;
class Player;
class Logger;

class AuctionService {
public:
    bool startAuction(
        PropertyPlot& property,
        const std::vector<Player*>& bidders,
        Logger& logger
    ) const;
};
