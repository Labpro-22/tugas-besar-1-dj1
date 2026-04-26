#include "models/Board/CardDeck.hpp"

#include "core/GameException.hpp"

template<class T>
CardDeck<T>::CardDeck() {
    this->drawPile = {};
    this->discardPile = {};
}

template<class T>
void CardDeck<T>::shuffle() {
    random_device random;
    mt19937 g(random());

    std::shuffle(drawPile.begin(), drawPile.end(), g);
}

// asumsi discard pile tidak kosong
template<class T>
void CardDeck<T>::reshuffleFromDiscard() {
    if (discardPile.empty()) {
        throw InvalidStateException("Tidak dapat reshuffle karena discard pile kosong.");
    }

    // Pindahkan semua kartu dari discard pile ke draw pile
    while (!discardPile.empty()) {
        drawPile.push_back(std::move(discardPile.front()));
        discardPile.pop_front();
    }

    // Kocok ulang draw pile
    shuffle();
}

template<class T>
void CardDeck<T>::initialize(vector<T> card) {
    for (auto& card : cards) {
        drawPile.push_back(std::move(card));
    }

    discardPile.clear();
}

template<class T>
T CardDeck<T>::draw() {
    if (drawPile.empty()) {
        throw InvalidStateException("Draw pile kosong.");
    }
    T drawCard = std::move(drawPile.back());
    drawPile.pop_back();
    return drawCard;
}

template<class T>
void CardDeck<T>::discard(T card) {
    discardPile.push_back(std::move(card));
}

template<class T>
int CardDeck<T>::drawPileSize() {
    return drawPile.size();
}

template<class T>
int CardDeck<T>::discardPileSize() {
    return discardPile.size();
}

template<class T>
vector<T> CardDeck<T>::getAllCards() const {
    vector<T> deck;
    for(auto it = drawPile.begin(); it < drawPile.end(); ++it) {
        deck.push_back(*it);
    }
    return deck;
}

// Explicit instantiations
#include <memory>
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
#include "models/Card/SkillCard/SkillCard.hpp"

template class CardDeck<std::unique_ptr<ChanceCard>>;
template class CardDeck<std::unique_ptr<CommunityChestCard>>;
template class CardDeck<std::unique_ptr<SkillCard>>;

