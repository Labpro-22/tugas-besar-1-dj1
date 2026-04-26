#include "core/services/AuctionService.hpp"

#include <cctype>
#include <exception>
#include <string>
#include "core/services/CommandHandler.hpp"
#include "models/Player/Player.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
#include "utils/Logger.hpp"
#include "views/GameRenderer.hpp"

namespace {
std::string toUpper(std::string s) {
    for (char& c : s) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return s;
}

bool parseBidAmount(const std::string& raw, int& bidAmount) {
    try {
        std::size_t parsedCount = 0;
        const int parsed = std::stoi(raw, &parsedCount);
        if (parsedCount != raw.size()) {
            return false;
        }
        bidAmount = parsed;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

void logAuction(Logger& logger, const std::string& username, const std::string& detail) {
    logger.log(LogEntry{0, username, "AUCTION", detail});
}
}  // namespace

bool AuctionService::startAuction(
    PropertyPlot& property,
    const std::vector<Player*>& bidders,
    Logger& logger
) const {
    std::vector<Player*> active;
    for (Player* p : bidders) {
        if (p != nullptr && !p->isBankrupt()) {
            active.push_back(p);
        }
    }

    if (active.empty()) {
        logAuction(logger, "SYSTEM", "Lelang dibatalkan: tidak ada pemain yang ikut.");
        return false;
    }

    const std::string startingPlayer = active.front()->getUsername();
    GameRenderer::showAuctionStart(property, startingPlayer);
    logAuction(logger, "SYSTEM",
        "Lelang dimulai untuk " + property.getName() + ".");

    CommandHandler commandHandler;
    int highestBid = 0;
    Player* highestBidder = nullptr;

    auto auctionFinished = [&]() {
        if (active.empty()) return true;
        if (active.size() == 1 && highestBidder == active.front()) return true;
        return false;
    };

    while (!auctionFinished()) {
        bool changedThisRound = false;

        for (std::size_t i = 0; i < active.size();) {
            Player* bidder = active[i];

            if (active.size() == 1 && bidder == highestBidder) {
                break;
            }

            std::string bidderName = bidder->getUsername();
            GameRenderer::showAuctionTurn(bidderName);
            if (highestBidder != nullptr) {
                GameRenderer::showAuctionHighestBid(highestBid, highestBidder->getUsername());
            }

            const std::string action = toUpper(commandHandler.promptInput("Masukkan aksi [PASS/BID]"));

            bool removeFromActive = false;
            bool placedBid = false;

            if (action == "PASS") {
                logAuction(logger, bidder->getUsername(), "Pass dari lelang.");
                removeFromActive = true;
            } else if (action == "BID") {
                const std::string bidRaw = commandHandler.promptInput("Masukkan nominal BID");
                int bidAmount = 0;
                if (!parseBidAmount(bidRaw, bidAmount)) {
                    logAuction(logger, bidder->getUsername(), "Pass (input invalid).");
                    removeFromActive = true;
                } else if (bidAmount <= highestBid) {
                    logAuction(logger, bidder->getUsername(), "Pass (tawaran terlalu rendah).");
                    removeFromActive = true;
                } else if (bidAmount > bidder->getCash()) {
                    logAuction(logger, bidder->getUsername(), "Pass (uang tidak cukup).");
                    removeFromActive = true;
                } else {
                    highestBid = bidAmount;
                    highestBidder = bidder;
                    placedBid = true;
                    logAuction(logger, bidder->getUsername(),
                        "Menawar M" + std::to_string(bidAmount) + ".");
                }
            } else {
                logAuction(logger, bidder->getUsername(), "Pass (aksi tidak dikenali).");
                removeFromActive = true;
            }

            if (removeFromActive) {
                active.erase(active.begin() + static_cast<std::ptrdiff_t>(i));
                changedThisRound = true;
            } else {
                ++i;
                if (placedBid) changedThisRound = true;
            }

            if (auctionFinished()) break;
        }

        if (!changedThisRound) break;
    }

    if (highestBidder == nullptr) {
        logAuction(logger, "SYSTEM", "Lelang gagal: tidak ada penawaran.");
        return false;
    }

    highestBidder->buyProperty(property, highestBid);

    std::string winnerName = highestBidder->getUsername();
    GameRenderer::showAuctionResult(property, winnerName, highestBid);
    logAuction(logger, highestBidder->getUsername(),
        "Memenangkan lelang " + property.getName() + " seharga M" + std::to_string(highestBid) + ".");

    return true;
}
