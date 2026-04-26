#include "core/TurnManager.hpp"

#include <string>
#include <vector>

#include "core/GameException.hpp"
#include "core/SkillContext.hpp"
#include "core/services/AuctionService.hpp"
#include "core/services/BankruptcyService.hpp"
#include "core/services/CommandHandler.hpp"
#include "models/Board/Board.hpp"
#include "models/Board/CardDeck.hpp"
#include "models/Card/SkillCard/SkillCard.hpp"
#include "models/Player/Player.hpp"
#include "views/GameRenderer.hpp"

namespace {
int jailFineFor(const GameState& state) {
    return state.getJailFine() > 0 ? state.getJailFine() : TurnManager::JAIL_FINE;
}

std::vector<Player*> activeBidders(GameState& state, const Player* excluded = nullptr) {
    std::vector<Player*> bidders;
    for (Player& bidder : state.getPlayers()) {
        if (&bidder != excluded && !bidder.isBankrupt()) {
            bidders.push_back(&bidder);
        }
    }
    return bidders;
}
}  // namespace

void TurnManager::startTurn(Player& player, GameState& state) {
    player.resetTurnFlags();
    player.resetConsecutiveDoubles();

    if (player.isBankrupt()) {
        state.addLog(player.getUsername(), "TURN", "Pemain bangkrut dilewati.");
        return;
    }

    if (player.getStatus() == PlayerStatus::JAILED) {
        if (player.getJailTurns() <= 0) {
            state.addLog(player.getUsername(), "JAIL",
                "Sudah melewati batas giliran di penjara, wajib membayar denda.");
            payJailFine(player, state);
            return;
        }

        state.addLog(player.getUsername(), "JAIL",
            "Berada di penjara (" + std::to_string(player.getJailTurns()) +
            " giliran tersisa). Pilih PAY_JAIL_FINE atau ROLL.");
        return;
    }

    state.addLog(player.getUsername(), "TURN", "Mulai giliran.");
    CommandHandler commandHandler;
    drawSkillCardAtStart(player, state, commandHandler);
}

void TurnManager::endTurn(Player& player, GameState& state) {
    player.updateStatus();
    state.addLog(player.getUsername(), "TURN", "Selesai giliran.");
}

bool TurnManager::handleExtraTurn(Player& player, bool rolledDouble, GameState& state) {
    if (!rolledDouble || player.isBankrupt() || player.getStatus() == PlayerStatus::JAILED) {
        return false;
    }

    player.setHasRolled(false);
    state.addLog(player.getUsername(), "DOUBLE", "Mendapat giliran tambahan.");
    return true;
}

void TurnManager::advanceTurn(GameState& state) {
    state.nextPlayer();
    checkGameEndConditions(state);
}

void TurnManager::sendToJail(Player& player, GameState& state) {
    const int boardSize = state.getBoardSizeOrDefault();
    int jailIndex = state.getBoard().findPlotIndex(PlotType::PRISONPLOT);
    if (jailIndex < 0 || jailIndex >= boardSize) {
        jailIndex = 0;
    }

    player.moveTo(jailIndex, boardSize);
    player.goToJail();
    state.addLog(player.getUsername(), "JAIL", "Masuk penjara.");
}

bool TurnManager::payJailFine(Player& player, GameState& state) {
    if (player.getStatus() != PlayerStatus::JAILED) {
        return false;
    }

    const int fine = jailFineFor(state);
    BankruptcyService bankruptcyService;
    if (!bankruptcyService.canRecover(player, fine)) {
        bankruptcyService.liquidateAssets(player, fine - player.getCash(), state.getLogger());
    }
    if (!bankruptcyService.canRecover(player, fine)) {
        AuctionService auctionService;
        bankruptcyService.transferAssets(
            player,
            nullptr,
            state.getLogger(),
            activeBidders(state, &player),
            auctionService
        );
        checkGameEndConditions(state);
        return false;
    }

    player.pay(fine);
    player.setStatus(PlayerStatus::ACTIVE);
    player.setJailTurns(0);
    player.resetConsecutiveDoubles();
    state.addLog(player.getUsername(), "JAIL",
        "Membayar denda penjara sebesar M" + std::to_string(fine) + " dan bebas.");
    return true;
}

bool TurnManager::handleJailedRoll(Player& player, bool rolledDouble, GameState& state) {
    if (player.getStatus() != PlayerStatus::JAILED) {
        return true;
    }

    if (rolledDouble) {
        player.setStatus(PlayerStatus::ACTIVE);
        player.setJailTurns(0);
        player.resetConsecutiveDoubles();
        state.addLog(player.getUsername(), "JAIL", "Bebas dari penjara karena melempar double.");
        return true;
    }

    player.decrementJailTurns();
    if (player.getJailTurns() <= 0) {
        state.addLog(player.getUsername(), "JAIL",
            "Gagal melempar double tiga kali. Wajib membayar denda penjara.");
        return payJailFine(player, state);
    }

    state.addLog(player.getUsername(), "JAIL",
        "Gagal melempar double, tetap di penjara (" +
        std::to_string(player.getJailTurns()) + " giliran tersisa).");
    return false;
}

bool TurnManager::handleTripleDouble(Player& player, GameState& state) {
    if (player.getStatus() == PlayerStatus::JAILED) {
        return false;
    }
    if (player.getConsecutiveDoubles() < MAX_CONSECUTIVE_DOUBLES) {
        return false;
    }

    state.addLog(player.getUsername(), "DOUBLE",
        "Melempar double tiga kali berturut-turut dan langsung masuk penjara.");
    sendToJail(player, state);
    return true;
}

void TurnManager::grantGoSalary(Player& player, GameState& state) {
    const int goIndex = state.getBoard().findPlotIndex(PlotType::STARTPLOT);
    if (goIndex < 0 || player.getPosition() != goIndex) {
        return;
    }

    const int salary = state.getSalary();
    if (salary <= 0) {
        return;
    }

    player.receive(salary);
    state.addLog(player.getUsername(), "GO",
        "Mendarat di GO, menerima gaji M" + std::to_string(salary) + ".");
}

void TurnManager::handlePassedGo(Player& player, int oldPosition, int newPosition, GameState& state) {
    const int boardSize = state.getBoardSizeOrDefault();
    const int goIndex = state.getBoard().findPlotIndex(PlotType::STARTPLOT);
    if (goIndex < 0 || boardSize <= 0 || oldPosition == newPosition) {
        return;
    }

    const int distanceMoved = (newPosition - oldPosition + boardSize) % boardSize;
    const int distanceToGo = (goIndex - oldPosition + boardSize) % boardSize;
    if (distanceMoved == 0 || distanceToGo == 0 || distanceToGo > distanceMoved) {
        if (newPosition == goIndex) {
            grantGoSalary(player, state);
        }
        return;
    }

    const int salary = state.getSalary();
    if (salary <= 0) {
        return;
    }

    player.receive(salary);
    state.addLog(player.getUsername(), "GO",
        "Melewati GO, menerima gaji M" + std::to_string(salary) + ".");
}

bool TurnManager::checkGameEndConditions(GameState& state) {
    if (state.hasSingleActivePlayer()) {
        state.setGameOver(true);
        state.addLog("SYSTEM", "GAME_END", "Permainan selesai: satu pemain tersisa.");
        return true;
    }

    if (state.getMaxTurn() > 0 && state.getCurrentTurn() > state.getMaxTurn()) {
        state.setGameOver(true);
        state.addLog("SYSTEM", "GAME_END", "Permainan selesai: batas giliran tercapai.");
        return true;
    }

    return false;
}

void TurnManager::drawSkillCardAtStart(Player& player, GameState& state, CommandHandler& commandHandler) {
    if (player.isBankrupt()) {
        return;
    }

    auto& deck = state.getBoard().getSkillCardDeckPile();
    if (deck.drawPileSize() == 0) {
        if (deck.discardPileSize() == 0) {
            state.addLog(player.getUsername(), "KARTU", "Deck kartu kosong.");
            return;
        }
        deck.reshuffleFromDiscard();
    }

    std::shared_ptr<SkillCard> card = deck.draw();
    if (!card) {
        state.addLog(player.getUsername(), "KARTU", "Kartu kosong dilewati.");
        return;
    }

    const std::string cardName = card->getName();
    player.addOwnedCard(std::move(card));
    state.addLog(player.getUsername(), "KARTU", "Mendapat kartu " + cardName + ".");

    if (player.getOwnedCards().size() > 3) {
        handleCardOverflow(player, state, commandHandler, deck);
    }
}

void TurnManager::useCards(Player& player, GameState& state, CommandHandler& commandHandler) {
    if (player.hasUsedSkillThisTurn()) {
        GameRenderer::showHaveUsedSkillCard(true);
        return;
    }
    if (player.getHasRolled()) {
        GameRenderer::showHaveUsedSkillCard(false);
        return;
    }
    if (player.getOwnedCards().empty()) {
        state.addLog(player.getUsername(), "KARTU", "Tidak memiliki kartu kemampuan.");
        return;
    }

    for (std::size_t i = 0; i < player.getOwnedCards().size(); ++i) {
        const auto& card = player.getOwnedCards()[i];
        if (card) {
            GameRenderer::showCardList(static_cast<int>(i + 1), card->getName(), card->getDescription());
        }
    }

    const std::string raw = commandHandler.promptInput("Pilih nomor kartu (0 untuk batal)");
    int choice = 0;
    try {
        choice = std::stoi(raw);
    } catch (const std::exception&) {
        throw InvalidInputException("Nomor kartu tidak valid.");
    }

    if (choice == 0) {
        return;
    }
    if (choice < 1 || static_cast<std::size_t>(choice) > player.getOwnedCards().size()) {
        throw InvalidInputException("Nomor kartu tidak valid.");
    }

    const std::size_t index = static_cast<std::size_t>(choice - 1);
    const std::string cardName = player.getOwnedCards()[index]->getName();
    SkillContext ctx{player, state.getPlayers(), state.getBoard(), state.getLogger()};
    player.useCards(index, ctx);
    player.setUsedSkillThisTurn(true);
    state.addLog(player.getUsername(), "KARTU", "Menggunakan " + cardName + ".");
}

bool TurnManager::handleCardOverflow(
    Player& player,
    GameState& state,
    CommandHandler& commandHandler,
    CardDeck<std::shared_ptr<SkillCard>>& deck
) {
    if (player.getOwnedCards().size() <= 3) {
        return false;
    }

    const std::size_t cardIdx = commandHandler.promptCardDrop(player);
    if (cardIdx >= player.getOwnedCards().size()) {
        throw InvalidInputException("Nomor kartu tidak valid.");
    }

    const auto& card = player.getOwnedCards()[cardIdx];
    const std::string droppedName = card ? card->getName() : "Kartu kosong";
    player.dropCard(cardIdx, deck);
    state.addLog(player.getUsername(), "KARTU",
        "Membuang kartu " + droppedName + " karena tangan penuh.");
    return true;
}