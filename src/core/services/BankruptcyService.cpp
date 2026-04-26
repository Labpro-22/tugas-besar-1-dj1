#include "core/services/BankruptcyService.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "core/services/AuctionService.hpp"
#include "core/GameException.hpp"
#include "core/GameState.hpp"
#include "models/Player/Player.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "utils/Logger.hpp"

namespace {
void logBankruptcy(Logger& logger, const std::string& username, const std::string& detail) {
    logger.log(LogEntry{0, username, "BANKRUPTCY", detail});
}

std::vector<PropertyPlot*> getOwnedProperties(Player& player) {
    std::vector<PropertyPlot*> properties;
    for (const std::reference_wrapper<PropertyPlot>& propertyRef : player.getOwnedProperties()) {
        properties.push_back(&propertyRef.get());
    }
    return properties;
}

bool colorGroupHasBuilding(const Player& player, Color color) {
    for (const std::reference_wrapper<PropertyPlot>& propertyRef : player.getOwnedProperties()) {
        const PropertyPlot& property = propertyRef.get();
        const LandPlot* land = dynamic_cast<const LandPlot*>(&property);
        if (land != nullptr && land->getColor() == color && land->getLevel() > 0) {
            return true;
        }
    }
    return false;
}

bool canMortgage(const Player& player, const PropertyPlot& property) {
    if (property.getOwner() != &player || property.isMortgaged()) {
        return false;
    }

    const LandPlot* land = dynamic_cast<const LandPlot*>(&property);
    if (land == nullptr) {
        return true;
    }

    return !colorGroupHasBuilding(player, land->getColor());
}

int mortgagePotential(const Player& player) {
    int total = 0;
    for (const std::reference_wrapper<PropertyPlot>& propertyRef : player.getOwnedProperties()) {
        const PropertyPlot& property = propertyRef.get();
        if (canMortgage(player, property)) {
            total += property.getMortgageValue();
        }
    }
    return total;
}

int saleValueToBank(const PropertyPlot& property) {
    if (property.isMortgaged()) {
        return 0;
    }
    return property.calculateTotalValue();
}

int salePotential(const Player& player) {
    int total = 0;
    for (const std::reference_wrapper<PropertyPlot>& propertyRef : player.getOwnedProperties()) {
        total += saleValueToBank(propertyRef.get());
    }
    return total;
}

int recoveryPotential(const Player& player) {
    int total = 0;
    for (const std::reference_wrapper<PropertyPlot>& propertyRef : player.getOwnedProperties()) {
        const PropertyPlot& property = propertyRef.get();
        const int mortgageValue = canMortgage(player, property) ? property.getMortgageValue() : 0;
        const int saleValue = saleValueToBank(property);
        total += std::max(mortgageValue, saleValue);
    }
    return total;
}

void resetPropertyToBank(Player& owner, PropertyPlot& property) {
    if (LandPlot* land = dynamic_cast<LandPlot*>(&property)) {
        land->demolish();
    }
    property.endFestival();
    property.setOwner(nullptr);
    property.setPropertyStatus(PropertyStatus::BANK);
    owner.removeOwnedProperty(property);
}

std::vector<Player*> filterAuctionBidders(
    const std::vector<Player*>& bidders,
    const Player& bankrupt
) {
    std::vector<Player*> activeBidders;
    for (Player* bidder : bidders) {
        if (bidder != nullptr && bidder != &bankrupt && !bidder->isBankrupt()) {
            activeBidders.push_back(bidder);
        }
    }
    return activeBidders;
}

void transferToCreditor(Player& bankrupt, Player& creditor, Logger& logger) {
    const int remainingCash = bankrupt.getCash();
    if (remainingCash > 0) {
        creditor.receive(remainingCash);
        logBankruptcy(logger, bankrupt.getUsername(),
            "Uang tunai sisa M" + std::to_string(remainingCash) +
            " dialihkan ke " + creditor.getUsername() + ".");
    }

    std::vector<PropertyPlot*> properties = getOwnedProperties(bankrupt);
    for (PropertyPlot* property : properties) {
        if (property == nullptr || property->getOwner() != &bankrupt) {
            continue;
        }

        const std::string status = property->isMortgaged() ? "MORTGAGED" : "OWNED";
        bankrupt.transferProperty(*property, &creditor);
        logBankruptcy(logger, bankrupt.getUsername(),
            property->getName() + " (" + property->getCode() + ") " + status +
            " dialihkan ke " + creditor.getUsername() + ".");
    }

    bankrupt.setStatus(PlayerStatus::BANKRUPT);
    logBankruptcy(logger, bankrupt.getUsername(),
        "Dinyatakan bangkrut. Kreditor: " + creditor.getUsername() + ".");
}

void returnAssetsToBank(
    Player& bankrupt,
    Logger& logger,
    const std::vector<Player*>* auctionBidders,
    const AuctionService* auctionService
) {
    const int remainingCash = bankrupt.getCash();
    if (remainingCash > 0) {
        logBankruptcy(logger, bankrupt.getUsername(),
            "Uang tunai sisa M" + std::to_string(remainingCash) + " diserahkan ke Bank.");
    }

    std::vector<PropertyPlot*> properties = getOwnedProperties(bankrupt);
    std::vector<PropertyPlot*> auctionProperties;
    for (PropertyPlot* property : properties) {
        if (property == nullptr || property->getOwner() != &bankrupt) {
            continue;
        }

        const std::string propertyLabel = property->getName() + " (" + property->getCode() + ")";
        resetPropertyToBank(bankrupt, *property);
        auctionProperties.push_back(property);
        logBankruptcy(logger, bankrupt.getUsername(),
            propertyLabel + " dikembalikan ke Bank; bangunan dihancurkan dan status menjadi BANK.");
    }

    bankrupt.setStatus(PlayerStatus::BANKRUPT);
    logBankruptcy(logger, bankrupt.getUsername(), "Dinyatakan bangkrut. Kreditor: Bank.");

    if (auctionService == nullptr || auctionBidders == nullptr) {
        if (!auctionProperties.empty()) {
            logBankruptcy(logger, "SYSTEM",
                "Properti pemain bangkrut sudah kembali ke Bank dan menunggu lelang.");
        }
        return;
    }

    std::vector<Player*> activeBidders = filterAuctionBidders(*auctionBidders, bankrupt);
    if (activeBidders.empty()) {
        logBankruptcy(logger, "SYSTEM",
            "Lelang aset bankrut dibatalkan: tidak ada bidder aktif.");
        return;
    }

    for (PropertyPlot* property : auctionProperties) {
        if (property == nullptr) {
            continue;
        }
        logBankruptcy(logger, "SYSTEM",
            "Lelang aset bangkrut dimulai untuk " + property->getName() +
            " (" + property->getCode() + ").");
        auctionService->startAuction(*property, activeBidders, logger);
    }
}
}  // namespace

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

    const int totalMortgagePotential = mortgagePotential(player);
    const int totalSalePotential = salePotential(player);
    const int totalPotential = recoveryPotential(player);

    logBankruptcy(logger, player.getUsername(),
        "Kekurangan M" + std::to_string(amountNeeded) +
        ". Potensi gadai M" + std::to_string(totalMortgagePotential) +
        ", potensi jual ke Bank M" + std::to_string(totalSalePotential) +
        ", potensi pemulihan maksimum M" + std::to_string(totalPotential) + ".");

    if (totalPotential < amountNeeded) {
        logBankruptcy(logger, player.getUsername(),
            "Dana likuidasi tidak dapat menutup kewajiban. Total potensi M" +
            std::to_string(totalPotential) + ".");
        return 0;
    }

    int liquidated = 0;

    if (totalMortgagePotential >= amountNeeded) {
        std::vector<PropertyPlot*> properties = getOwnedProperties(player);
        std::sort(properties.begin(), properties.end(),
            [](const PropertyPlot* lhs, const PropertyPlot* rhs) {
                return lhs->getMortgageValue() < rhs->getMortgageValue();
            }
        );

        for (PropertyPlot* property : properties) {
            if (property == nullptr || !canMortgage(player, *property)) {
                continue;
            }

            const int value = property->getMortgageValue();
            property->setPropertyStatus(PropertyStatus::MORTGAGED);
            player.receive(value);
            liquidated += value;

            logBankruptcy(logger, player.getUsername(),
                property->getName() + " (" + property->getCode() +
                ") digadaikan. Menerima M" + std::to_string(value) + ".");

            if (liquidated >= amountNeeded) {
                break;
            }
        }
    } else {
        struct LiquidationChoice {
            PropertyPlot* property;
            int value;
            bool mortgage;
        };

        std::vector<LiquidationChoice> choices;
        for (PropertyPlot* property : getOwnedProperties(player)) {
            if (property == nullptr) {
                continue;
            }

            const int mortgageValue = canMortgage(player, *property) ? property->getMortgageValue() : 0;
            const int saleValue = saleValueToBank(*property);
            if (mortgageValue <= 0 && saleValue <= 0) {
                continue;
            }

            choices.push_back(LiquidationChoice{
                property,
                std::max(mortgageValue, saleValue),
                mortgageValue >= saleValue
            });
        }

        std::sort(choices.begin(), choices.end(),
            [](const LiquidationChoice& lhs, const LiquidationChoice& rhs) {
                return lhs.value < rhs.value;
            }
        );

        for (const LiquidationChoice& choice : choices) {
            PropertyPlot* property = choice.property;
            if (property == nullptr || property->getOwner() != &player) {
                continue;
            }

            const std::string propertyLabel = property->getName() + " (" + property->getCode() + ")";
            if (choice.mortgage && canMortgage(player, *property)) {
                const int value = property->getMortgageValue();
                property->setPropertyStatus(PropertyStatus::MORTGAGED);
                player.receive(value);
                liquidated += value;

                logBankruptcy(logger, player.getUsername(),
                    propertyLabel + " digadaikan. Menerima M" + std::to_string(value) + ".");
            } else {
                const int value = saleValueToBank(*property);
                if (value <= 0) {
                    continue;
                }

                player.receive(value);
                resetPropertyToBank(player, *property);
                liquidated += value;

                logBankruptcy(logger, player.getUsername(),
                    propertyLabel + " dijual ke Bank. Menerima M" + std::to_string(value) + ".");
            }

            if (liquidated >= amountNeeded) {
                break;
            }
        }
    }

    logBankruptcy(logger, player.getUsername(),
        "Dana likuidasi terkumpul M" + std::to_string(liquidated) + ".");
    return liquidated;
}

void BankruptcyService::transferAssets(Player& bankrupt, Player* creditor, Logger& logger) const {
    if (creditor != nullptr && creditor != &bankrupt && !creditor->isBankrupt()) {
        transferToCreditor(bankrupt, *creditor, logger);
        return;
    }

    returnAssetsToBank(bankrupt, logger, nullptr, nullptr);
}

void BankruptcyService::transferAssets(
    Player& bankrupt,
    Player* creditor,
    Logger& logger,
    const std::vector<Player*>& auctionBidders,
    const AuctionService& auctionService
) const {
    if (creditor != nullptr && creditor != &bankrupt && !creditor->isBankrupt()) {
        transferToCreditor(bankrupt, *creditor, logger);
        return;
    }

    returnAssetsToBank(bankrupt, logger, &auctionBidders, &auctionService);
}
