#pragma once
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
 
class CampaignCard : public CommunityChestCard {
    public:
        CampaignCard() = default;
        ~CampaignCard() = default;
        string getName();
        string getDescription();
        void activate(SkillContext& ctx);
};
 