#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "core/Command.hpp"

class GameState;
class EffectResolver;
class TurnManager;
class Player;

class CommandHandler {
public:
    std::unique_ptr<Command> parse(const std::string& rawCommand) const;
    bool execute(const Command& command, GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const;
    static bool promptYesNo(std::string prompt);
    std::size_t promptCardDrop(const Player& player);
    static std::string promptInput(std::string prompt);
};
