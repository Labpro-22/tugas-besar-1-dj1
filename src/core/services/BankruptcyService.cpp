#include "core/services/BankruptcyService.hpp"

#include "core/GameException.hpp"
#include "core/GameState.hpp"
#include "models/Player/Player.hpp"
#include "utils/Logger.hpp"

bool BankruptcyService::canRecover(const Player& player, int amountNeeded) const {
    if (amountNeeded < 0) {
        throw InvalidInputException("amountNeeded tidak boleh negatif.");
    }
    return player.getCash() >= amountNeeded;
}

int BankruptcyService::liquidateAssets(Player& player, int amountNeeded, Logger& logger) const {
    if (amountNeeded < 0) {
        throw InvalidInputException("amountNeeded tidak boleh negatif.");
    }
    if (amountNeeded == 0) {
        return 0;
    }

    logger.log(LogEntry{
        0,
        player.getUsername(),
        "BANKRUPTCY",
        "Mencoba likuidasi aset, tetapi belum ada aset terdaftar untuk dijual."
    });
    return 0;
}

void BankruptcyService::transferAssets(Player& bankrupt, Player* creditor, Logger& logger) const {
    const int remainingCash = bankrupt.getCash();
    if (creditor != nullptr && creditor != &bankrupt && !creditor->isBankrupt() && remainingCash > 0) {
        creditor->receive(remainingCash);
    }

    bankrupt.setStatus(PlayerStatus::BANKRUPT);
    logger.log(LogEntry{
        0,
        bankrupt.getUsername(),
        "BANKRUPTCY",
        "Dinyatakan bangkrut."
    });
}
