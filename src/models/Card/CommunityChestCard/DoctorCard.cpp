#include "models/Card/CommunityChestCard/DoctorCard.hpp"

#include "core/GameException.hpp"
#include "views/GameRenderer.hpp"
 
string DoctorCard::getName() {
    return "DoctorCard";
}
 
string DoctorCard::getDescription() {
    return "Biaya dokter. Bayar M700.";
}
 
void DoctorCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    try {
        currPlayer.pay(700);
        GameRenderer::showOnLandCommunityChestCard(*this, 700, currPlayer.getCash());
        if (currPlayer.getCash() < 700) {
            // TODO: Handle Bankrupt
        } 
    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}
 