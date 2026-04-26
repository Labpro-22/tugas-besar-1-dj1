#include "models/Card/SkillCard/ShieldCard.hpp"

void ShieldCard::activate(SkillContext& ctx) {
    try {
        ctx.getCurrentPlayer().setShieldActive(true);
        ctx.getCurrentPlayer().setUsedSkillThisTurn(true);
        ctx.getCurrentPlayer().setShieldTurnLeft(1);

        GameRenderer::showActivateSkillCard(ShieldCard::getName(), "Anda kebal terhadap tagihan atau sanksi selama giliran ini.");
    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}

const string ShieldCard::getName() const {
    return "ShieldCard";
}

const string ShieldCard::getDescription() const {
    return "Melindungi pemain dari tagihan sewa maupun sanksi apapun yang merugikan selama 1 giliran.";
}