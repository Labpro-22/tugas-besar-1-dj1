#include "core/GameEngine.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "core/GameException.hpp"
#include "views/GameRenderer.hpp"
#include "utils/SaveLoader.hpp"

namespace {
constexpr int MIN_PLAYERS = 2;
constexpr int MAX_PLAYERS = 4;

std::string trim(const std::string& text) {
    const std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

int readPlayerCount() {
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
            if (parsed == input.size() && value >= MIN_PLAYERS && value <= MAX_PLAYERS) {
                return value;
            }
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

bool tryLoadGame(GameEngine& engine) {
    while (true) {
        std::string filename = CommandHandler::promptInput(
            "Masukkan nama file save (file harus ada di folder data/)");
        filename = trim(filename);

        if (filename.empty()) {
            std::cout << "Nama file tidak boleh kosong.\n";
            continue;
        }

        if (!SaveLoader::fileExists(filename)) {
            GameRenderer::failLoadFile();
            if (!CommandHandler::promptYesNo("Coba file lain?")) {
                return false;
            }
            continue;
        }

        try {
            SaveLoader::load(engine.getState(), filename);
            std::string currentPlayer = engine.getState().getCurrentPlayer().getUsername();
            GameRenderer::successLoadFile(filename, currentPlayer);
            return true;
        } catch (const std::exception&) {
            GameRenderer::failLoadFile();
            if (!CommandHandler::promptYesNo("Coba file lain?")) {
                return false;
            }
        }
    }
}

} // namespace

int main() {
    try {
        GameEngine engine;

        GameRenderer::showGameTitle();

        bool loaded = false;
        if (CommandHandler::promptYesNo("Apakah anda ingin memuat permainan tersimpan?")) {
            loaded = tryLoadGame(engine);
            if (!loaded) {
                if (!CommandHandler::promptYesNo("Mulai permainan baru?")) {
                    return 0;
                }
            }
        }

        if (!loaded) {
            const int playerCount = readPlayerCount();
            const std::vector<std::string> playerNames = readPlayerNames(playerCount);
            engine.startNewGame(playerNames);
        }

        
        GameRenderer::showGameStartHint();
        std::string lastPlayerName;
        while (!engine.isGameOver()) {
            const GameState& state = engine.getState();
            const std::string& currentPlayerName = state.getCurrentPlayer().getUsername();

            if (currentPlayerName != lastPlayerName) {
                GameRenderer::showBoard(state);
                lastPlayerName = currentPlayerName;
            }

            GameRenderer::showTurnHeader(
                state.getCurrentTurn(),
                state.getMaxTurn(),
                currentPlayerName
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
