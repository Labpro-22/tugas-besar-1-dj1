#pragma once
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
 
class DoctorCard : public CommunityChestCard {
    public:
        DoctorCard() = default;
        ~DoctorCard() = default;
        string getName();
        string getDescription();
        void activate(SkillContext& ctx);
};
 