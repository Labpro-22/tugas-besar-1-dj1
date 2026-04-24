#pragma once

#include <memory>
#include <string>
#include "core/Command.hpp"

class CommandHandler {
public:
    std::unique_ptr<Command> parse(const std::string& rawCommand) const;
    bool execute(const Command& command, GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const;
    static bool promptYesNo(std::string prompt);
    std::size_t promptCardDrop(const Player& player);
    static std::string promptInput(std::string prompt);
    template <typename Validator>
    std::string CommandHandler::promptInput(std::string prompt, Validator isValid){
        std::string answer;
        while (true){
            std::cout << prompt << ": "; //TODO: tampilkan dengan renderer
            std::cin >> answer;

            if (isValid(answer)){
                return answer;
            }
        }
    }
};
