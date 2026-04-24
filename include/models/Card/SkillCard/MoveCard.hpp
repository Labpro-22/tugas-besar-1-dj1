#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"
#include "models/Board/Board.hpp"

class MoveCard : public SkillCard {
    private:
        int moveNumber;
    public:
        MoveCard();
        MoveCard(int moveNumber);
        ~MoveCard() = default;
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
        int getMoveNumber() const;
};


