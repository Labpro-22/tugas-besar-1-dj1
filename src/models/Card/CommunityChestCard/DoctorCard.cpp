#include "models/Card/CommunityChestCard/DoctorCard.hpp"

#include "core/GameException.hpp"
#include "core/services/BankruptcyService.hpp"
#include "core/services/AuctionService.hpp"
#include "views/GameRenderer.hpp"
#include "models/Player/Player.hpp"
 
string DoctorCard::getName() {
    return "DoctorCard";
}
 
string DoctorCard::getDescription() {
    return "Biaya dokter. Bayar M700.";
}
 
void DoctorCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    const int amount = 700;
    try {
        if (currPlayer.isShieldActive() || currPlayer.getCash() >= amount) {
            currPlayer.pay(amount);
            GameRenderer::showOnLandCommunityChestCard(*this, amount, currPlayer.getCash());
            return;
        }

        BankruptcyService bankruptcyService;
        bankruptcyService.liquidateAssets(currPlayer, amount - currPlayer.getCash(), ctx.getLogger());

        if (currPlayer.getCash() >= amount) {
            currPlayer.pay(amount);
            GameRenderer::showOnLandCommunityChestCard(*this, amount, currPlayer.getCash());
            return;
        }

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

    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}
 