#include "models/Card/SkillCard/DemolitionCard.hpp"

void DemolitionCard::activate(SkillContext& ctx) {    
    try {
        GameRenderer::showActivateSkillCard(DemolitionCard::getName(), "Kamu dapat memilih 1 plot lawan untuk di HANCURKAN");
        string index = CommandHandler::promptInput("Pilih index plot lawan yang mau di-HANCURKAN");
        ctx.getBoard().getPlot(stoi(index))->demolish();
        ctx.getCurrentPlayer().setUsedSkillThisTurn(true);
    } catch(const GameException& e) {
        GameRenderer::throwException(e);
    }   
}

const string DemolitionCard::getName() const {
    return "DemolitionCard";
}

const string DemolitionCard::getDescription() const {
    return "Menghancurkan satu properti milik pemain lawan.";
}