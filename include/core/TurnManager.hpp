#pragma once

#include <memory>
#include "core/GameState.hpp"

class CommandHandler;
class SkillCard;
template <class T> class CardDeck;

class TurnManager {
public:
    static constexpr int MAX_JAIL_TURNS = 3;
    static constexpr int MAX_CONSECUTIVE_DOUBLES = 3;
    static constexpr int JAIL_FINE = 50;

    void startTurn(Player& player, GameState& state);
    void endTurn(Player& player, GameState& state);
    bool handleExtraTurn(Player& player, bool rolledDouble, GameState& state);
    void advanceTurn(GameState& state);

    void sendToJail(Player& player, GameState& state);
    bool payJailFine(Player& player, GameState& state);
    bool handleJailedRoll(Player& player, bool rolledDouble, GameState& state);

    bool handleTripleDouble(Player& player, GameState& state);
    void grantGoSalary(Player& player, GameState& state);
    void handlePassedGo(Player& player, int oldPosition, int newPosition, GameState& state);
    bool checkGameEndConditions(GameState& state);
    void drawSkillCardAtStart(Player& player, GameState& state, CommandHandler& commandHandler);
    void useCards(Player& player, GameState& state, CommandHandler& commandHandler);
    bool handleCardOverflow(Player& player, GameState& state, CommandHandler& commandHandler, CardDeck<std::shared_ptr<SkillCard>>& deck);
};
