#include "models/Card/ChanceCard/PrisonCard.hpp"

#include "core/GameException.hpp"
#include "core/SkillContext.hpp"
#include "models/Board/Board.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

const string PrisonCard::getName() const {
    return "PrisonCard";
}

const string PrisonCard::getDescription() const{
    return "Masuk Penjara";
}

void PrisonCard::activate(SkillContext& ctx) {
    ctx.getCurrentPlayer().sendToJail(ctx.getBoard());
    GameRenderer::showOnLandChanceCard(*this, "Kamu dipindahkan ke PENJARA (PEN) selama 3 Turn");
}



