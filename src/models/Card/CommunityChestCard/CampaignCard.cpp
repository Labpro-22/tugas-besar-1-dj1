#include "models/Card/CommunityChestCard/CampaignCard.hpp"

string CampaignCard::getName() {
    return "CampaignCard";
}
 
string CampaignCard::getDescription() {
    return "Anda mau nyaleg. Bayar M200 kepada setiap pemain.";
}
 
void CampaignCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    
    try{
        for (Player& other : ctx.getPlayers()) {
            if(currPlayer.getCash() < 200) break;
            if (other.getUsername() == currPlayer.getUsername()) continue;
            if (other.isBankrupt()) continue;
            
            currPlayer.pay(200);
            other.receive(200);
        }
        
        GameRenderer::showOnLandCommunityChestCard(*this, 200, currPlayer.getCash());
        if(currPlayer.getCash() < 200) {
            // TODO: Handle Bankrupt
        }

    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}