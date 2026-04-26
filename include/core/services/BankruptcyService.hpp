#pragma once

#include <vector>

class AuctionService;
class GameState;
class Player;
class Logger;

class BankruptcyService {
public:
    bool canRecover(const Player& player, int amountNeeded) const;
    int liquidateAssets(Player& player, int amountNeeded, Logger& logger) const;
    void transferAssets(Player& bankrupt, Player* creditor, Logger& logger) const;
    void transferAssets(
        Player& bankrupt,
        Player* creditor,
        Logger& logger,
        const std::vector<Player*>& auctionBidders,
        const AuctionService& auctionService
    ) const;
};

using BankcruptService = BankruptcyService;
