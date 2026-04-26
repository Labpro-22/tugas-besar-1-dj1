#include "models/Card/CommunityChestCard/CampaignCard.hpp"

#include "core/GameException.hpp"
#include "core/services/BankruptcyService.hpp"
#include "core/services/AuctionService.hpp"
#include "views/GameRenderer.hpp"
#include "models/Player/Player.hpp"

string CampaignCard::getName() {
    return "CampaignCard";
}

string CampaignCard::getDescription() {
    return "Anda mau nyaleg. Bayar M200 kepada setiap pemain.";
}

void CampaignCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    std::vector<Player*> targets;
    for (Player& other : ctx.getPlayers()) {
        if (other.getUsername() != currPlayer.getUsername() && !other.isBankrupt()) {
            targets.push_back(&other);
        }
    }

    const int amountPerPlayer = 200;
    const int totalAmount = static_cast<int>(targets.size()) * amountPerPlayer;

    try {
        if (totalAmount > 0) {
            if (currPlayer.getCash() < totalAmount) {
                BankruptcyService bankruptcyService;
                bankruptcyService.liquidateAssets(currPlayer, totalAmount - currPlayer.getCash(), ctx.getLogger());

                if (currPlayer.getCash() < totalAmount) {
                    // Handle Bankruptcy
                    std::vector<Player*> auctionBidders;
                    for (Player& bidder : ctx.getPlayers()) {
                        auctionBidders.push_back(&bidder);
                    }

                    AuctionService auctionService;
                    bankruptcyService.transferAssets(
                        currPlayer,
                        nullptr,
                        ctx.getLogger(),
                        auctionBidders,
                        auctionService
                    );

                    GameRenderer::showOnLandCommunityChestCard(*this, totalAmount, currPlayer.getCash());
                    return;
                }
            }

            // Pay each player
            for (Player* other : targets) {
                currPlayer.pay(amountPerPlayer);
                other->receive(amountPerPlayer);
            }
        }

        GameRenderer::showOnLandCommunityChestCard(*this, totalAmount, currPlayer.getCash());

    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}