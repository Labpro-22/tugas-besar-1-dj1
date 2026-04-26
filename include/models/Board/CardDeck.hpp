#pragma once
#include "iostream"
#include "deque"
#include "vector"
#include "random"
#include "algorithm"
#include "views/GameRenderer.hpp"
using namespace std;

template<class T>
class CardDeck {
    private:
        deque<T> drawPile;
        deque<T> discardPile;
    public:
        CardDeck();
        ~CardDeck() = default;

        void shuffle();
        void initialize(vector<T>);

        T draw();
        void discard(T);
        void reshuffleFromDiscard();

        int drawPileSize();
        int discardPileSize();

        vector<T> getAllCards() const;
};

