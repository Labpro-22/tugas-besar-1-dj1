#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"
#include "models/Board/Board.hpp"

class TeleportCard : public SkillCard {
    public:
        TeleportCard() = default;
        ~TeleportCard() = default;
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
};
