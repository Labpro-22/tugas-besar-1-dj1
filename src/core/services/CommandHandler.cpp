#include "core/services/CommandHandler.hpp"

#include <cctype>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include "core/GameException.hpp"
#include "core/GameState.hpp"
#include "core/TurnManager.hpp"
#include "core/services/EffectResolver.hpp"
#include "models/Player/Player.hpp"
#include "models/Card/SkillCard/SkillCard.hpp"

using std::cout;
using std::cin;
using std::endl;

namespace {
std::string toUpper(std::string text) {
    for (char& ch : text) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return text;
}

int parseInt(const std::string& value) {
    try {
        std::size_t parsedCount = 0;
        const int result = std::stoi(value, &parsedCount);
        if (parsedCount != value.size()) {
            throw InvalidInputException("Argumen numerik tidak valid: " + value);
        }
        return result;
    } catch (const std::exception&) {
        throw InvalidInputException("Argumen numerik tidak valid: " + value);
    }
}
}  // namespace

std::unique_ptr<Command> CommandHandler::parse(const std::string& rawCommand) const {
    std::istringstream iss(rawCommand);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        throw InvalidInputException("Perintah kosong.");
    }

    const std::string cmd = toUpper(tokens[0]);
    if (cmd == "ROLL") {
        const int boardSize = (tokens.size() > 1) ? parseInt(tokens[1]) : 40;
        return std::make_unique<RollDiceCommand>(boardSize);
    }
    if (cmd == "SETDICE" || cmd == "SET_DICE") {
        if (tokens.size() < 3) {
            throw InvalidInputException("SETDICE membutuhkan 2 argumen angka.");
        }
        return std::make_unique<SetDiceCommand>(parseInt(tokens[1]), parseInt(tokens[2]));
    }
    if (cmd == "PRINT_BOARD") {
        return std::make_unique<PrintBoardCommand>();
    }
    if (cmd == "PRINT_DEED") {
        if (tokens.size() < 2) {
            throw InvalidInputException("PRINT_DEED membutuhkan kode properti.");
        }
        return std::make_unique<PrintDeedCommand>(tokens[1]);
    }
    if (cmd == "PRINT_PROPERTY") {
        return std::make_unique<PrintPropertyCommand>();
    }
    if (cmd == "BUILD") {
        if (tokens.size() < 2) {
            throw InvalidInputException("BUILD membutuhkan kode properti.");
        }
        return std::make_unique<BuildCommand>(tokens[1]);
    }
    if (cmd == "MORTGAGE") {
        if (tokens.size() < 2) {
            throw InvalidInputException("MORTGAGE membutuhkan kode properti.");
        }
        return std::make_unique<MortgageCommand>(tokens[1]);
    }
    if (cmd == "REDEEM") {
        if (tokens.size() < 2) {
            throw InvalidInputException("REDEEM membutuhkan kode properti.");
        }
        return std::make_unique<RedeemCommand>(tokens[1]);
    }
    if (cmd == "USE_SKILL") {
        if (tokens.size() < 2) {
            throw InvalidInputException("USE_SKILL membutuhkan index kartu.");
        }
        return std::make_unique<UseSkillCardCommand>(parseInt(tokens[1]));
    }
    if (cmd == "SAVE") {
        if (tokens.size() < 2) {
            throw InvalidInputException("SAVE membutuhkan nama file.");
        }
        return std::make_unique<SaveCommand>(tokens[1]);
    }
    if (cmd == "TAX") {
        if (tokens.size() < 2) {
            throw InvalidInputException("TAX membutuhkan nominal pajak.");
        }
        return std::make_unique<TaxCommand>(parseInt(tokens[1]));
    }
    if (cmd == "EARN") {
        if (tokens.size() < 2) {
            throw InvalidInputException("EARN membutuhkan nominal uang.");
        }
        return std::make_unique<EarnCommand>(parseInt(tokens[1]));
    }
    if (cmd == "BANKRUPT" || cmd == "BANKCRUPT") {
        const int creditorIdx = (tokens.size() > 1) ? parseInt(tokens[1]) : -1;
        return std::make_unique<BankruptCommand>(creditorIdx);
    }
    if (cmd == "END") {
        return std::make_unique<EndTurnCommand>();
    }
    if (cmd == "PAY_JAIL_FINE" || cmd == "PAY_FINE" || cmd == "PAYJAIL") {
        return std::make_unique<PayJailFineCommand>();
    }
    throw InvalidInputException("Perintah tidak dikenali: " + tokens[0]);
}

bool CommandHandler::execute(const Command& command, GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const {
    return command.execute(state, effectResolver, turnManager);
}

std::string CommandHandler::promptInput(std::string prompt){
    GameRenderer::showInputMessage(prompt);
    std::string answer;
    std::cin >> answer;
    return answer;
}

bool CommandHandler::promptYesNo(std::string prompt){
    GameRenderer::showYesNoMessage(prompt);
    while(true){
        try{
            std::cout << prompt << " (y/n): ";

            std::string answer;
            std::cin >> answer;
            if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes" || answer == "YES"){
                return true;
            }
            else if (answer == "n" || answer == "N" || answer == "no" || answer == "No" || answer == "NO"){
                return false;
            }
            else{
                throw InvalidInputException();
            }
        } catch (const GameException& e){
            GameRenderer::throwException(e);
        }
    }
    return true;
}

// CommandHandler.cpp
std::size_t CommandHandler::promptCardDrop(const Player& player) {
    const auto& cards = player.getOwnedCards();

    while (true) {
        try {
            Formatter::dropCardWarning(cards.at(3)->getName());
            for (std::size_t i = 0; i < cards.size(); ++i) {
                GameRenderer::showCardList(i, cards.at(i)->getName(), cards.at(i)->getDescription());
            }

            std::string raw = promptInput("Pilih nomor kartu (1-"+ std::to_string(cards.size()) + ")");
            int choice = parseInt(raw);

            if (choice < 1 || static_cast<std::size_t>(choice) > cards.size()) {
                throw InvalidInputException("Nomor tidak valid.");
            }

            GameRenderer::showDropCardAction(cards.at(choice)->getName());
            return static_cast<std::size_t>(choice - 1);

        } catch (const GameException& e) {
            GameRenderer::throwException(e);
        }
    }
}
