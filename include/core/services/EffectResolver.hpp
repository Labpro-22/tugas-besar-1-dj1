#pragma once

#include <vector>
#include "core/services/AuctionService.hpp"
#include "core/services/BankruptcyService.hpp"

class GameState;
class Player;
class PropertyPlot;

class EffectResolver {
private:
    AuctionService auctionService;
    BankruptcyService bankruptcyService;

public:
    void resolveLanding(Player& player, int tileIndex, GameState& state);
    void resolveBankruptcy(Player& player, Player* creditor, GameState& state) const;
    void resolveTax(Player& player, int taxAmount, GameState& state) const;
    bool resolveAuction(PropertyPlot& property, const std::vector<Player*>& bidders, GameState& state) const;
};
