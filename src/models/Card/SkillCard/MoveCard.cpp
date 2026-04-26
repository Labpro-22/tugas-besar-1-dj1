#include "models/Card/SkillCard/MoveCard.hpp"
#include "core/GameException.hpp"

void MoveCard::activate(SkillContext& ctx) {
    random_device random;
    mt19937 g(random());
    uniform_int_distribution<> number(1,12);
    int moveNumber = number(g);

    Player& currPlayer = ctx.getCurrentPlayer();
    int boardSize = ctx.getBoard().getSize();

    try {
        currPlayer.move(moveNumber, boardSize);
        currPlayer.setUsedSkillThisTurn(true);

        std::ostringstream oss;
        oss << "Kamu pindah " << moveNumber << " Petak ke depan";
        GameRenderer::showActivateSkillCard(MoveCard::getName(), oss.str());

    } catch (const GameException& e) {
        GameRenderer::throwException(e);
    }
}

const string MoveCard::getName() const {
    return "MoveCard";
}

const string MoveCard::getDescription() const {
    return "Memungkinkan pemain untuk bergerak maju sekian petak. Nominal langkah pergerakan ditentukan secara acak pada saat pemain pertama kali mendapatkan kartu ini.";
}
