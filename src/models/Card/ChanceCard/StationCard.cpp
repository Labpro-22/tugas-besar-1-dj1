#include "models/Card/ChanceCard/StationCard.hpp"
#include "core/GameException.hpp"

const string StationCard::getName() const{
    return "StationCard";
}

const string StationCard::getDescription() const{
    return "Pergi ke stasiun terdekat";
}

void StationCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    Board& board = ctx.getBoard();
    int boardSize = board.getSize();
    int currPosition = currPlayer.getPosition();

    PlotType currType = board.getPlot(currPosition)->getType();
    while(currType != PlotType::STATIONPLOT) {
        currType = board.getPlot(currPosition)->getType();
        currPosition++;
        currPosition %= boardSize;
    }

    int index = currPosition;
    try {
        currPlayer.moveTo(index, boardSize);
        std::ostringstream oss;
        oss << "Kamu pindah ke Stasiun" << board.getPlot(currPlayer.getPosition())->getName();
        GameRenderer::showOnLandChanceCard(*this, oss.str());
    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}



