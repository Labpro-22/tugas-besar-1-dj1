#include "core/PlotContext.hpp"
#include "models/Player/Player.hpp"
#include "models/Board/Board.hpp"
#include "utils/Logger.hpp"
#include "core/services/AuctionService.hpp"

PlotContext::PlotContext(Player& currentPlayer, std::vector<Player>& players, Board& board, Dice& dice, Logger& logger, AuctionService& auctionService)
    : currentPlayer(currentPlayer), players(players), board(board), dice(dice), logger(logger), auctionService(auctionService) {}

Player& PlotContext::getCurrentPlayer(){
    return currentPlayer;
}

std::vector<Player>& PlotContext::getPlayers(){
    return players;
}

Board& PlotContext::getBoard(){
    return board;
}

Dice& PlotContext::getDice(){
    return dice;
}

Logger& PlotContext::getLogger(){
    return logger;
}

AuctionService& PlotContext::getAuctionService(){
    return auctionService;
}
