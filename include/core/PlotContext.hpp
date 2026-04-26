#pragma once

#include <vector>

class Player;
class Board;
class Dice;
class Logger;
class AuctionService;

class PlotContext {
private:
    Player& currentPlayer;
    std::vector<Player>& players;
    Board& board;
    Dice& dice;
    Logger& logger;
    AuctionService& auctionService;

public:
    PlotContext(Player& currentPlayer, std::vector<Player>& players, Board& board, Dice& dice, Logger& logger, AuctionService& auctionService);
    Player& getCurrentPlayer();
    std::vector<Player>& getPlayers();
    Board& getBoard();
    Dice& getDice();
    Logger& getLogger();
    AuctionService& getAuctionService();
};
