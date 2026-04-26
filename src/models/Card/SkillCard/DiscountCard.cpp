#include "models/Card/SkillCard/DiscountCard.hpp"

DiscountCard::DiscountCard() {
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> dist(1, 100);
    discountValue = dist(g);
}

DiscountCard::DiscountCard(int discountValue) : discountValue(discountValue) {}

void DiscountCard::activate(SkillContext& ctx) {
    ctx.getCurrentPlayer().setDiscountValue(discountValue);
    ctx.getCurrentPlayer().setDiscountTurnLeft(1);
    ctx.getCurrentPlayer().setUsedSkillThisTurn(true);

    std::ostringstream oss;
    oss << "Kamu mendapatkan diskon sebesar " << discountValue << "%" << "(1 Turn)";
    GameRenderer::showActivateSkillCard(DiscountCard::getName(), oss.str());
}

const string DiscountCard::getName() const {
    return "DiscountCard";
}

const string DiscountCard::getDescription() const {
    return "Memberikan diskon dengan persentase acak kepada pemain saat kartu didapatkan. Masa berlaku DiscountCard adalah 1 giliran.";
}

int DiscountCard::getDiscountValue() const {
    return discountValue;
}