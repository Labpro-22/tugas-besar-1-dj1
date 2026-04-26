#pragma once

#include <vector>

class GameState;
class Player;

class AuctionService {
public:
    bool startAuction(
        int basePrice,
        const std::vector<Player*>& bidders,
        Player*& winner,
        int& finalPrice,
        GameState& state
    ) const;
};
