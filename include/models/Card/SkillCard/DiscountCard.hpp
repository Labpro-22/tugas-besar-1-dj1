#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DiscountCard : public SkillCard {
    private:
        int discountValue;
    public:
        DiscountCard();
        DiscountCard(int discountValue);
        ~DiscountCard() = default;
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
        int getDiscountValue() const; 

};
