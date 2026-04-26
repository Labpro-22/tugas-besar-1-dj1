#include "core/GameEngine.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "core/GameException.hpp"
#include "views/GameRenderer.hpp"

namespace {

std::string trim(const std::string& text) {
    const std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

int readPositiveInt() {
    while (true) {
        GameRenderer::showPromptPlayerCount();
        std::string input;
        if (!std::getline(std::cin, input)) {
            throw InvalidInputException("Input dihentikan sebelum permainan dimulai.");
        }
        input = trim(input);
        try {
            std::size_t parsed = 0;
            const int value = std::stoi(input, &parsed);
            if (parsed == input.size() && value > 0) return value;
        } catch (const std::exception&) {}
        GameRenderer::showInvalidIntInput();
    }
}

std::vector<std::string> readPlayerNames(int count) {
    std::vector<std::string> names;
    names.reserve(static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        while (true) {
            GameRenderer::showPromptPlayerName(i + 1);
            std::string name;
            if (!std::getline(std::cin, name)) {
                throw InvalidInputException("Input nama pemain dihentikan.");
            }
            name = trim(name);
            if (name.empty()) {
                GameRenderer::showPlayerNameEmpty();
                continue;
            }
            if (std::find(names.begin(), names.end(), name) != names.end()) {
                GameRenderer::showPlayerNameDuplicate();
                continue;
            }
            names.push_back(name);
            break;
        }
    }
    return names;
}

} // namespace

int main() {
    try {
        GameEngine engine;

        GameRenderer::showGameTitle();
        const int playerCount = readPositiveInt();
        const std::vector<std::string> playerNames = readPlayerNames(playerCount);
        engine.startNewGame(playerNames);

        GameRenderer::showGameStartHint();
        while (!engine.isGameOver()) {
            const GameState& state = engine.getState();
            GameRenderer::showBoard(state);
            GameRenderer::showTurnHeader(
                state.getCurrentTurn(),
                state.getMaxTurn(),
                state.getCurrentPlayer().getUsername()
            );

            GameRenderer::showCommandPrompt();
            std::string command;
            if (!std::getline(std::cin, command)) {
                GameRenderer::showInputStopped();
                break;
            }

            command = trim(command);
            if (command.empty()) {
                continue;
            }
            if (command == "EXIT") {
                break;
            }

            try {
                engine.run({command});
                const auto logs = engine.getState().getRecentLogs(1);
                if (!logs.empty()) {
                    GameRenderer::showLogger(logs.back());
                }
            } catch (const GameException& e) {
                GameRenderer::throwException(e);
            }
        }

        const std::vector<Player> winners = engine.getWinner();
        if (winners.empty()) {
            GameRenderer::showNoWinner();
            return 0;
        }
        if (winners.size() == 1) {
            std::string name = winners.front().getUsername();
            GameRenderer::showWinner(name);
            return 0;
        }
        GameRenderer::showDrawResult(winners);
        return 0;

    } catch (const GameException& e) {
        GameRenderer::showFatalError(e.what());
        return 1;
    } catch (const std::exception& e) {
        GameRenderer::showFatalError(e.what());
        return 1;
    }
}