#include "core/GameState.hpp"

#include <stdexcept>

GameState::GameState(int maxTurn)
    : currentPlayerIdx(0), maxTurn(maxTurn), currentTurn(1), gameOver(false) {
    if (maxTurn <= 0) {
        throw std::invalid_argument("maxTurn harus lebih dari 0.");
    }
}

void GameState::addPlayer(const Player& player) {
    players.push_back(player);
    clampCurrentPlayerIndex();
}

std::vector<Player>& GameState::getPlayers() {
    return players;
}

const std::vector<Player>& GameState::getPlayers() const {
    return players;
}

Player& GameState::getCurrentPlayer() {
    if (players.empty()) {
        throw std::runtime_error("Tidak ada pemain dalam game state.");
    }
    clampCurrentPlayerIndex();
    return players[static_cast<std::size_t>(currentPlayerIdx)];
}

const Player& GameState::getCurrentPlayer() const {
    if (players.empty()) {
        throw std::runtime_error("Tidak ada pemain dalam game state.");
    }

    int safeIdx = currentPlayerIdx;
    if (safeIdx < 0 || safeIdx >= static_cast<int>(players.size())) {
        safeIdx = 0;
    }
    return players[static_cast<std::size_t>(safeIdx)];
}

int GameState::getCurrentPlayerIdx() const {
    return currentPlayerIdx;
}

int GameState::getMaxTurn() const {
    return maxTurn;
}

int GameState::getCurrentTurn() const {
    return currentTurn;
}

void GameState::setCurrentPlayerIdx(int idx) {
    currentPlayerIdx = idx;
    clampCurrentPlayerIndex();
}

void GameState::setCurrentTurn(int turn) {
    if (turn <= 0) {
        throw std::invalid_argument("currentTurn harus lebih dari 0.");
    }
    currentTurn = turn;
}

void GameState::nextPlayer() {
    if (players.empty()) {
        return;
    }

    const int startingIdx = currentPlayerIdx;
    do {
        currentPlayerIdx = (currentPlayerIdx + 1) % static_cast<int>(players.size());
        if (currentPlayerIdx == 0) {
            ++currentTurn;
        }

        if (!players[static_cast<std::size_t>(currentPlayerIdx)].isBankrupt()) {
            return;
        }
    } while (currentPlayerIdx != startingIdx);

    gameOver = true;
}

std::vector<Player> GameState::getActivePlayers() const {
    std::vector<Player> activePlayers;
    for (const Player& player : players) {
        if (!player.isBankrupt()) {
            activePlayers.push_back(player);
        }
    }
    return activePlayers;
}

bool GameState::hasSingleActivePlayer() const {
    int activeCount = 0;
    for (const Player& player : players) {
        if (!player.isBankrupt()) {
            ++activeCount;
            if (activeCount > 1) {
                return false;
            }
        }
    }
    return activeCount == 1;
}

void GameState::addLog(const std::string& entry) {
    logs.push_back(entry);
}

const std::vector<std::string>& GameState::getLogs() const {
    return logs;
}

Dice& GameState::getDice() {
    return dice;
}

const Dice& GameState::getDice() const {
    return dice;
}

bool GameState::isGameOver() const {
    return gameOver;
}

void GameState::setGameOver(bool value) {
    gameOver = value;
}

void GameState::clampCurrentPlayerIndex() {
    if (players.empty()) {
        currentPlayerIdx = 0;
        return;
    }
    if (currentPlayerIdx < 0 || currentPlayerIdx >= static_cast<int>(players.size())) {
        currentPlayerIdx = 0;
    }
}
