#include "models/Board/Dice.hpp"

Dice::Dice() : dice1(0), dice2(0), isManual(false) {}

Dice::~Dice() {}

std::pair<int, int> Dice::roll() {
    if (!isManual) {
        random_device random;
        mt19937 gen(random());
        uniform_int_distribution<int> distribution(1, 6);

        dice1 = distribution(gen);
        dice2 = distribution(gen);
    }
    isManual = false;
    return {dice1, dice2};
}

void Dice::setDiceManual(int d1, int d2) {
    dice1 = d1;
    dice2 = d2;
    isManual = true;
}

void Dice::resetManual() {
    isManual = false;
}

bool Dice::isDouble() const {
    return dice1 == dice2;
}

int Dice::getDice1() const {
    return dice1;
}

int Dice::getDice2() const {
    return dice2;
}  

int Dice::getTotal() const {
    return dice1 + dice2;
}

bool Dice::getIsManual() const {
    return isManual;
} 
