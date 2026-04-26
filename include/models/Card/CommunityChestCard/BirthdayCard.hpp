#pragma once
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"

class BirthdayCard : public CommunityChestCard {
    public:
        BirthdayCard() = default;
        ~BirthdayCard() = default;
        string getName();
        string getDescription();
        void activate(SkillContext& ctx);
};