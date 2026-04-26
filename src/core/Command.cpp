#include "core/Command.hpp"

#include <utility>
#include "core/GameException.hpp"
#include "core/GameState.hpp"
#include "core/TurnManager.hpp"
#include "core/services/EffectResolver.hpp"
#include "core/SkillContext.hpp"
#include "views/GameRenderer.hpp"
#include "models/Player/Player.hpp"
#include "utils/ConfigSaver.hpp"
#include "views/GameRenderer.hpp"

// ── LEMPAR_DADU (command 2) ──────────────────────────────────────────
RollDiceCommand::RollDiceCommand(int boardSize) : boardSize(boardSize) {
    if (boardSize <= 0) {
        throw InvalidInputException("boardSize harus lebih dari 0.");
    }
}

bool RollDiceCommand::execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const {
    if (state.getPlayers().empty()) {
        state.addLog("Tidak ada pemain untuk melempar dadu.");
        return false;
    }

    Player& player = state.getCurrentPlayer();
    if (player.getHasRolled()) {
        state.addLog(player.getUsername() + " sudah melakukan ROLL pada giliran ini.");
        return false;
    }

    const std::pair<int, int> dicePair = state.getDice().roll();
    const int steps = dicePair.first + dicePair.second;
    const int effectiveBoardSize = state.getBoardSizeOrDefault(boardSize);
    const bool isDouble = state.getDice().isDouble();

    state.addLog(player.getUsername() + " melempar dadu: " +
        std::to_string(dicePair.first) + " dan " + std::to_string(dicePair.second) + ".");
    player.setHasRolled(true);

    if (player.getStatus() == PlayerStatus::JAILED) {
        const bool released = turnManager.handleJailedRoll(player, isDouble, state);
        if (!released) {
            std::string position = state.getBoard().getPlot(player.getPosition())->getName();
            GameRenderer::showDiceRoll(player, state.getDice(), position);
            return true;
        }

        const int oldPosition = player.getPosition();
        player.move(steps, effectiveBoardSize);
        turnManager.handlePassedGo(player, oldPosition, player.getPosition(), state);
        std::string position = state.getBoard().getPlot(player.getPosition())->getName();
        GameRenderer::showDiceRoll(player, state.getDice(), position);
        effectResolver.resolveLanding(player, player.getPosition(), state);
        return true;
    }

    if (isDouble) {
        player.incrementConsecutiveDoubles();
        if (turnManager.handleTripleDouble(player, state)) {
            std::string position = state.getBoard().getPlot(player.getPosition())->getName();
            GameRenderer::showDiceRoll(player, state.getDice(), position);
            return true;
        }
    } else {
        player.resetConsecutiveDoubles();
    }

    const int oldPosition = player.getPosition();
    player.move(steps, effectiveBoardSize);
    turnManager.handlePassedGo(player, oldPosition, player.getPosition(), state);
 
    std::string position = state.getBoard().getPlot(player.getPosition())->getName();
    GameRenderer::showDiceRoll(player, state.getDice(), position);
 
    effectResolver.resolveLanding(player, player.getPosition(), state);

    if (player.getStatus() != PlayerStatus::JAILED) {
        turnManager.handleExtraTurn(player, isDouble, state);
    }
    return true;
}

SetDiceCommand::SetDiceCommand(int dice1, int dice2) : dice1(dice1), dice2(dice2) {}

bool SetDiceCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.getDice().setDiceManual(dice1, dice2);
    state.addLog("Dadu manual diset ke " + std::to_string(dice1) + " dan " + std::to_string(dice2) + ".");
    return true;
}

bool PrintBoardCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    const Board& board = state.getBoard();
    state.addLog(
        "Perintah PRINT_BOARD dijalankan. Jumlah tile board: " +
        std::to_string(board.getSize()) + "."
    );
    return true;
}

PrintDeedCommand::PrintDeedCommand(std::string code) : code(std::move(code)) {}

bool PrintDeedCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah PRINT_DEED untuk properti " + code + " dijalankan.");
    Plot* plot = state.getBoard().getPlotByCode(code);
    if (!plot) {
        cout << Formatter::deedNotFound(code);
        return true;
    }

    LandPlot* land = dynamic_cast<LandPlot*>(plot);
    if (!land) {
        cout << Formatter::deedNotFound(code);
        return true;
    }

    GameRenderer::showDeed(*land);
    return true;
}

bool PrintPropertyCommand::execute(GameState& state, EffectResolver& eff, TurnManager&) const {
    state.addLog("Perintah PRINT_PROPERTY dijalankan.");
    AuctionService auctionService;
    PlotContext ctx(state.getCurrentPlayer(), state.getPlayers(), state.getBoard(), state.getDice(), state.getLogger(), auctionService);
    GameRenderer::showPropertyList(ctx, state.getCurrentPlayer());
    return true;
}

BuildCommand::BuildCommand(std::string propertyCode) : propertyCode(std::move(propertyCode)) {}

bool BuildCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah BUILD untuk properti " + propertyCode + " dijalankan.");
    AuctionService auctionService;
    PlotContext ctx(state.getCurrentPlayer(), state.getPlayers(), state.getBoard(), state.getDice(), state.getLogger(), auctionService);
    GameRenderer::showBuildGroupList(state.getCurrentPlayer());
    const auto& ownedProperties = state.getCurrentPlayer().getOwnedProperties();

    map<Color, vector<LandPlot*>> colorGroups;
    for (const auto& plotRef : ownedProperties) {
        Plot& plot = plotRef.get();

        auto* land = dynamic_cast<LandPlot*>(&plot);
        if (!land) continue;

        colorGroups[land->getColor()].push_back(land);
    }

    map<Color, vector<LandPlot*>> eligibleGroups;
    for (const auto& pair : colorGroups) {
        const vector<LandPlot*>& plots = pair.second;

        bool monopolyFulfilled = true;

        for (const LandPlot* land : plots) {
            if (land->getPropertyStatus() != PropertyStatus::OWNED) {
                monopolyFulfilled = false;
                break;
            }
        }

        if (plots.empty()) {
            monopolyFulfilled = false;
        }

        if (monopolyFulfilled) {
            eligibleGroups[pair.first] = plots;
        }
    }

    if (eligibleGroups.empty()) {
        GameRenderer::showBuildNoEligible();
        return true;
    }

    vector<Color> colorOptions;
    for (const auto& pair : colorGroups) {
        colorOptions.push_back(pair.first);
    }

    if (colorOptions.empty()) {
        GameRenderer::throwException(InvalidInputException("Tidak ada color group yang bisa dibangun."));
        return false;
    }

    Color selectedColor;
    vector<LandPlot*> selectedPlots;

    try {
        string index = CommandHandler::promptInput("Pilih nomor color group (0 untuk batal)");
        int colorIndex = stoi(index);

        if (colorIndex == 0) {
            return true;
        }

        if (colorIndex < 1 || colorIndex > static_cast<int>(colorOptions.size())) {
            throw InvalidInputException();
        }

        selectedColor = colorOptions[colorIndex - 1];
        selectedPlots = colorGroups[selectedColor];
        GameRenderer::showBuildPlotList(state.getCurrentPlayer(), selectedColor);

    } catch (const GameException& e) {
        GameRenderer::throwException(e);
        return false;
    } catch (...) {
        GameRenderer::throwException(InvalidInputException());
        return false;
    }

    try {
        string petak = CommandHandler::promptInput("Pilih petak (0 untuk batal)");
        int plotIndex = stoi(petak);

        if (plotIndex == 0) {
            return true;
        }

        if (plotIndex < 1 || plotIndex > static_cast<int>(selectedPlots.size())) {
            throw InvalidInputException();
        }

        LandPlot* selectedLand = selectedPlots[plotIndex - 1];
        if (selectedLand->getLevel() == 5) {
            throw InvalidInputException();
        }

        bool allFourHouses = true;
        for (const LandPlot* land : selectedPlots) {
            if (land->getLevel() != 4) {
                allFourHouses = false;
                break;
            }
        }

        if (allFourHouses) {
            int upgradeCost = selectedLand->getUpgHotelPrice();
            if (state.getCurrentPlayer().getCash() < upgradeCost) {
                throw InvalidInputException("Uang tidak cukup untuk upgrade ke hotel.");
            }

            bool confirm = CommandHandler::promptYesNo("Upgrade ke hotel?");
            if (!confirm) {
                return true;
            }

            state.getCurrentPlayer().pay(upgradeCost);
            selectedLand->build(ctx);

            GameRenderer::showBuildUpgradeSuccess(state.getCurrentPlayer(), *selectedLand);
            return true;
        }

        int minLevel = selectedPlots[0]->getLevel();
        for (const LandPlot* land : selectedPlots) {
            if (land->getLevel() < minLevel) {
                minLevel = land->getLevel();
            }
        }

        if (selectedLand->getLevel() > minLevel) {
            throw InvalidInputException("Petak belum memenuhi syarat pemerataan untuk dibangun.");
        }

        int buildCost = selectedLand->getUpgHousePrice();
        if (state.getCurrentPlayer().getCash() < buildCost) {
            throw InvalidInputException("Uang tidak cukup untuk membangun rumah.");
        }

        state.getCurrentPlayer().pay(buildCost);
        selectedLand->build(ctx);
        GameRenderer::showBuildSuccess(state.getCurrentPlayer(),*selectedLand);

    } catch (const GameException& e) {
        GameRenderer::throwException(e);
        return false;
    } catch (...) {
        GameRenderer::throwException(InvalidInputException());
        return false;
    }
    return true;
}

MortgageCommand::MortgageCommand(std::string propertyCode) : propertyCode(std::move(propertyCode)) {}

bool MortgageCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah MORTGAGE untuk properti " + propertyCode + " dijalankan.");
    Player& player = state.getCurrentPlayer();
    const auto& ownedProperties = player.getOwnedProperties();

    vector<PropertyPlot*> mortgageOptions;
    int idx = 1;
    for (const auto& plotRef : ownedProperties) {
        Plot& plot = plotRef.get();
        auto* property = dynamic_cast<PropertyPlot*>(&plot);
        if (!property) {
            continue;
        }

        if (property->getPropertyStatus() == PropertyStatus::MORTGAGED) {
            continue;
        }

        auto* land = dynamic_cast<LandPlot*>(property);
        if (land && land->getLevel() > 0) {
            GameRenderer::showMortgageFailed(*land);
            int buildingIdx = 1;

            for (const auto& otherRef : ownedProperties) {
                Plot& otherPlot = otherRef.get();

                auto* otherLand = dynamic_cast<LandPlot*>(&otherPlot);
                if (!otherLand) {
                    continue;
                }

                if (otherLand->getColor() != land->getColor()) {
                    continue;
                }

                if (otherLand->getLevel() <= 0) {
                    continue;
                }

                GameRenderer::showSellBuildingOption(buildingIdx++, *otherLand);
            }

            continue;
        }

        mortgageOptions.push_back(property);
        GameRenderer::showCanMortgagedList(*property, property->getMortgageValue());
        idx++;
    }

    if (mortgageOptions.empty()) {
        GameRenderer::throwException(InvalidInputException("Tidak ada properti yang dapat digadaikan."));
        return false;
    }

    int choice = 0;
    try {
        string input = CommandHandler::promptInput("Pilih properti yang ingin digadaikan (0 untuk batal)");
        std::size_t parsedCount = 0;
        choice = stoi(input, &parsedCount);
        if (parsedCount != input.size()) {
            throw InvalidInputException();
        }
    } catch (const GameException& e) {
        GameRenderer::throwException(e);
        return false;
    } catch (...) {
        GameRenderer::throwException(InvalidInputException());
        return false;
    }

    if (choice == 0) {
        return true;
    }

    if (choice < 1 || choice > static_cast<int>(mortgageOptions.size())) {
        GameRenderer::throwException(InvalidInputException());
        return false;
    }

    PropertyPlot* selectedProperty = mortgageOptions[choice - 1];
    int mortgageValue = selectedProperty->getMortgageValue();
    selectedProperty->setPropertyStatus(PropertyStatus::MORTGAGED);

    player.receive(mortgageValue);
    GameRenderer::showMortgageSuccess(selectedProperty->getName(), mortgageValue, player.getCash());

    state.addLog(player.getUsername() + " menggadaikan " + selectedProperty->getName()+ " dan menerima " + Formatter::moneyString(mortgageValue) + ".");
    return true;
}

RedeemCommand::RedeemCommand(std::string propertyCode) : propertyCode(std::move(propertyCode)) {}

bool RedeemCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    Player& player = state.getCurrentPlayer();
    const auto& ownedProps = player.getOwnedProperties();

    std::vector<std::reference_wrapper<PropertyPlot>> mortgaged;
    for (const auto& ref : ownedProps) {
        if (ref.get().getPropertyStatus() == PropertyStatus::MORTGAGED) {
            mortgaged.push_back(ref.get());
        }
    }

    if (mortgaged.empty()) {
        GameRenderer::showRedeemNoEligible();
        return false;
    }

    GameRenderer::showRedeemListHeader(player.getCash());
    for (const auto& ref : mortgaged) {
        GameRenderer::showRedeemList(ref.get());
    }
    GameRenderer::showRedeemListFooter();

    std::string rawChoice = CommandHandler::promptInput("");
    int choice = 0;
    try { choice = std::stoi(rawChoice); } catch (...) { return false; }
    if (choice == 0) return false;
    if (choice < 1 || static_cast<std::size_t>(choice) > mortgaged.size()) return false;

    PropertyPlot& chosen = mortgaged[static_cast<std::size_t>(choice) - 1].get();
    int redeemPrice = chosen.getBuyPrice();

    if (player.getCash() < redeemPrice) {
        GameRenderer::showRedeemFailed(chosen, player.getCash());
        return false;
    }

    player.pay(redeemPrice);
    chosen.setPropertyStatus(PropertyStatus::OWNED);
    GameRenderer::showRedeemSuccess(chosen, player.getCash());
    state.addLog(player.getUsername(), "TEBUS",
        "Tebus " + chosen.getName() + " seharga M" + std::to_string(redeemPrice));
    return true;
}

UseSkillCardCommand::UseSkillCardCommand(int cardIndex) : cardIndex(cardIndex) {
    if (cardIndex < 0) {
        throw InvalidInputException("cardIndex tidak boleh negatif.");
    }
}

bool UseSkillCardCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    Player& player = state.getCurrentPlayer();
 
    if (player.hasUsedSkillThisTurn()) {
        GameRenderer::showHaveUsedSkillCard(true);
        return false;
    }
 
    if (player.getHasRolled()) {
        GameRenderer::showHaveUsedSkillCard(false);
        return false;
    }
 
    const auto& cards = player.getOwnedCards();
    if (cards.empty()) {
        state.addLog(player.getUsername() + " tidak memiliki kartu kemampuan.");
        return false;
    }
 
    GameRenderer::showCardList(0, "", ""); 
    for (std::size_t i = 0; i < cards.size(); ++i) {
        GameRenderer::showCardList(static_cast<int>(i) + 1,
            cards[i]->getName(), cards[i]->getDescription());
    }
 
    if (cardIndex < 1 || static_cast<std::size_t>(cardIndex) > cards.size()) {
        throw InvalidInputException("Index kartu tidak valid.");
    }
 
    const std::string name = cards[static_cast<std::size_t>(cardIndex) - 1]->getName();
    const std::string desc = cards[static_cast<std::size_t>(cardIndex) - 1]->getDescription();
 
    SkillContext ctx{player, state.getPlayers(), state.getBoard(), state.getLogger()};
    player.useCards(static_cast<std::size_t>(cardIndex) - 1, ctx);
    player.setUsedSkillThisTurn(true);
 
    GameRenderer::showActivateSkillCard(name, desc);
    state.addLog(player.getUsername(), "KARTU",
        "Pakai " + name + " → " + desc);
    return true;
}

SaveCommand::SaveCommand(std::string filename) : filename(std::move(filename)) {}

bool SaveCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    if (state.getCurrentPlayer().getHasRolled()) {
        state.addLog("SIMPAN hanya bisa dilakukan sebelum melempar dadu.");
        return false;
    }
 
    if (ConfigSaver::fileExists(filename)) {
        std::string prompt = "File \"" + filename + "\" sudah ada. Timpa file lama?";
        if (!CommandHandler::promptYesNo(prompt)) {
            return false;
        }
    }
 
    try {
        ConfigSaver::save(state, filename);
        GameRenderer::successSaveFile(const_cast<std::string&>(filename));
        state.addLog(state.getCurrentPlayer().getUsername(), "SIMPAN",
            "Permainan disimpan ke " + filename);
        return true;
    } catch (const std::exception&) {
        GameRenderer::failSaveFile(const_cast<std::string&>(filename));
        return false;
    }
}

TaxCommand::TaxCommand(int amount) : amount(amount) {
    if (amount < 0) {
        throw InvalidInputException("Nominal pajak tidak boleh negatif.");
    }
}

bool TaxCommand::execute(GameState& state, EffectResolver& effectResolver, TurnManager&) const {
    effectResolver.resolveTax(state.getCurrentPlayer(), amount, state);
    return true;
}

EarnCommand::EarnCommand(int amount) : amount(amount) {
    if (amount < 0) {
        throw InvalidInputException("Nominal uang tidak boleh negatif.");
    }
}

bool EarnCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    Player& player = state.getCurrentPlayer();
    player.receive(amount);
    state.addLog(player.getUsername() + " menerima uang sebesar " + std::to_string(amount) + ".");
    return true;
}

BankruptCommand::BankruptCommand(int creditorIdx) : creditorIdx(creditorIdx) {}

bool BankruptCommand::execute(GameState& state, EffectResolver& effectResolver, TurnManager&) const {
    Player* creditor = nullptr;
    if (creditorIdx >= 0 && creditorIdx < static_cast<int>(state.getPlayers().size()) && creditorIdx != state.getCurrentPlayerIdx()) {
        creditor = &state.getPlayers()[static_cast<std::size_t>(creditorIdx)];
    }

    effectResolver.resolveBankruptcy(state.getCurrentPlayer(), creditor, state);
    return true;
}

bool EndTurnCommand::execute(GameState& state, EffectResolver&, TurnManager& turnManager) const {
    Player& currentPlayer = state.getCurrentPlayer();
    if (currentPlayer.getConsecutiveDoubles() != 0 || !currentPlayer.getHasRolled()){
        throw InvalidInputException("Tidak bisa mengakhiri turn sekarang.");
    }
    turnManager.endTurn(currentPlayer, state);
    turnManager.advanceTurn(state);

    if (!state.isGameOver()) {
        turnManager.startTurn(state.getCurrentPlayer(), state);
    }
    return true;
}

bool PayJailFineCommand::execute(GameState& state, EffectResolver&, TurnManager& turnManager) const {
    Player& player = state.getCurrentPlayer();
    if (player.getStatus() != PlayerStatus::JAILED) {
        state.addLog(player.getUsername() + " tidak sedang berada di penjara.");
        return false;
    }
    if (player.getHasRolled()) {
        state.addLog(player.getUsername() + " harus membayar denda sebelum melempar dadu.");
        return false;
    }
    return turnManager.payJailFine(player, state);
}
