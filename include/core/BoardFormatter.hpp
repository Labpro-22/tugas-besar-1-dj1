#pragma once

#include <string>
#include <vector>

#include "models/Plot/Colors.hpp"

class Board;
class Player;

class BoardFormatter {
private:
    static const int CELL_WIDTH = 16;
    static const int CELL_HEIGHT = 4;

    static std::string fgColor(Color color);
    static std::string bgColor(Color color);
    static std::string ansiReset();
    static std::string colorCode(Color color);

    static int visibleLength(const std::string& text);
    static std::string padRight(const std::string& text, int width);
    static std::string centerText(const std::string& text, int width);
    static std::string paintCellLine(const std::string& text, Color color);

    static std::vector<std::string> emptyCell();
    static std::vector<std::string> makeCellLines(int plotIndex, const Board& board, const std::vector<Player>& players);

    static std::string makeSeparator(int cellCount);
    static void calculateSides(int totalPlots, int& topCount, int& rightCount, int& bottomCount, int& leftCount);

    static std::string getOwnerLabel(const std::string& ownerName, const std::vector<Player>& players);
    static std::string getPlayerLabels(int plotIndex, const Board& board, const std::vector<Player>& players);

    static std::vector<std::string> makeCenterInfo(const std::vector<Player>& players, int currentTurn, int maxTurn, int width, int height);

public:
    static std::string boardFormat(const Board& board, const std::vector<Player>& players, int currentTurn, int maxTurn);
};