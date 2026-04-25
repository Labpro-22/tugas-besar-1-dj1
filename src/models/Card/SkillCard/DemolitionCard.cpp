#include "models/Card/SkillCard/DemolitionCard.hpp"

void DemolitionCard::activate(SkillContext& ctx) {
    ctx.getBoard().getPlot(ctx.getCurrentPlayer().getPosition())->demolish();
}

const string DemolitionCard::getName() const {
    return "DemolitionCard";
}

const string DemolitionCard::getDescription() const {
    return "Menghancurkan satu properti milik pemain lawan.";
}