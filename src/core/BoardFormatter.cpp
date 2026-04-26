#include "core/BoardFormatter.hpp"

#include "models/Board/Board.hpp"
#include "models/Player/Player.hpp"
#include "models/Player/PlayerStatus.hpp"
#include "models/Plot/Plot.hpp"
#include "models/Plot/PlotType.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"

#include <sstream>
#include <algorithm>

using namespace std;

string BoardFormatter::fgColor(Color color) {
    switch (color) {
        case Color::BROWN:     return "\033[38;5;130m";
        case Color::LIGHTBLUE: return "\033[96m";
        case Color::PINK:      return "\033[95m";
        case Color::GRAY:      return "\033[90m";
        case Color::ORANGE:    return "\033[38;5;208m";
        case Color::RED:       return "\033[91m";
        case Color::YELLOW:    return "\033[93m";
        case Color::GREEN:     return "\033[92m";
        case Color::DARKBLUE:  return "\033[94m";
        default:               return "\033[97m";
    }
}

string BoardFormatter::bgColor(Color color) {
    switch (color) {
        case Color::BROWN:     return "\033[48;5;94m\033[97m";
        case Color::LIGHTBLUE: return "\033[48;5;117m\033[30m";
        case Color::PINK:      return "\033[48;5;213m\033[30m";
        case Color::GRAY:      return "\033[48;5;245m\033[30m";
        case Color::ORANGE:    return "\033[48;5;208m\033[30m";
        case Color::RED:       return "\033[48;5;196m\033[97m";
        case Color::YELLOW:    return "\033[48;5;226m\033[30m";
        case Color::GREEN:     return "\033[48;5;82m\033[30m";
        case Color::DARKBLUE:  return "\033[48;5;21m\033[97m";
        default:               return "\033[40m\033[97m";
    }
}

string BoardFormatter::ansiReset() {
    return "\033[0m";
}

string BoardFormatter::colorCode(Color color) {
    switch (color) {
        case Color::BROWN:     return "CK";
        case Color::LIGHTBLUE: return "BM";
        case Color::PINK:      return "PK";
        case Color::GRAY:      return "AB";
        case Color::ORANGE:    return "OR";
        case Color::RED:       return "MR";
        case Color::YELLOW:    return "KN";
        case Color::GREEN:     return "HJ";
        case Color::DARKBLUE:  return "BT";
        default:               return "DF";
    }
}

int BoardFormatter::visibleLength(const string& text) {
    int length = 0;
    bool inAnsi = false;

    for (int i = 0; i < static_cast<int>(text.length()); i++) {
        if (text[i] == '\033') {
            inAnsi = true;
            continue;
        }

        if (inAnsi) {
            if (text[i] == 'm') {
                inAnsi = false;
            }
            continue;
        }

        length++;
    }

    return length;
}

string BoardFormatter::padRight(const string& text, int width) {
    int visible = visibleLength(text);

    if (visible >= width) {
        return text;
    }

    return text + string(width - visible, ' ');
}

string BoardFormatter::centerText(const string& text, int width) {
    int visible = visibleLength(text);

    if (visible >= width) {
        return text;
    }

    int left = (width - visible) / 2;
    int right = width - visible - left;

    return string(left, ' ') + text + string(right, ' ');
}

string BoardFormatter::paintCellLine(const string& text, Color color) {
    string padded = padRight(text, CELL_WIDTH);
    string bg = bgColor(color);

    if (bg.empty()) {
        return padded;
    }

    return bg + padded + ansiReset();
}

vector<string> BoardFormatter::emptyCell() {
    vector<string> lines;

    for (int i = 0; i < CELL_HEIGHT; i++) {
        lines.push_back(string(CELL_WIDTH, ' '));
    }

    return lines;
}

string BoardFormatter::getOwnerLabel(const string& ownerName, const vector<Player>& players) {
    for (int i = 0; i < static_cast<int>(players.size()); i++) {
        if (players[i].getUsername() == ownerName) {
            return "P" + to_string(i + 1);
        }
    }

    return "";
}

string BoardFormatter::getPlayerLabels(int plotIndex, const Board& board, const vector<Player>& players) {
    string label;
    int prisonIndex = board.findPlotIndex(PlotType::PRISONPLOT);

    for (int i = 0; i < static_cast<int>(players.size()); i++) {
        const Player& player = players[i];

        if (player.isBankrupt()) continue;
        if (player.getPosition() != plotIndex) continue;

        string playerLabel;

        if (plotIndex == prisonIndex) {
            if (player.getStatus() == PlayerStatus::JAILED) {
                playerLabel = "P" + to_string(i + 1) + ":IN";
            } else {
                playerLabel = "P" + to_string(i + 1) + ":V";
            }
        } else {
            playerLabel = "(" + to_string(i + 1) + ")";
        }

        if (visibleLength(label + playerLabel) <= CELL_WIDTH) {
            label += playerLabel;
        }
    }

    return label;
}

vector<string> BoardFormatter::makeCellLines(int plotIndex, const Board& board, const vector<Player>& players) {
    const auto& plots = board.getPlots();

    if (plotIndex < 0 || plotIndex >= static_cast<int>(plots.size())) {
        return emptyCell();
    }

    Plot* plot = plots[plotIndex].get();
    Color color = plot->getColor();

    string codeLine = to_string(plotIndex) + " [" + colorCode(color) + "] " + plot->getCode();
    string ownerLine;
    string buildLine;
    string playerLine = getPlayerLabels(plotIndex, board, players);

    PropertyPlot* property = dynamic_cast<PropertyPlot*>(plot);

    if (property != nullptr) {
        Player* owner = property->getOwner();

        if (owner != nullptr) {
            ownerLine = getOwnerLabel(owner->getUsername(), players);
        }

        if (property->isMortgaged()) {
            if (!ownerLine.empty()) ownerLine += " ";
            ownerLine += "[M]";
        }

        LandPlot* land = dynamic_cast<LandPlot*>(property);

        if (land != nullptr) {
            int level = land->getLevel();

            if (level >= 1 && level <= 4) {
                buildLine = string(level, '^');
            } else if (level == 5) {
                buildLine = "*";
            }
        }
    }

    vector<string> lines;

    lines.push_back(paintCellLine(codeLine, color));
    lines.push_back(paintCellLine(ownerLine, color));
    lines.push_back(paintCellLine(buildLine, color));
    lines.push_back(paintCellLine(playerLine, color));

    return lines;
}

string BoardFormatter::makeSeparator(int cellCount) {
    string separator = "+";

    for (int i = 0; i < cellCount; i++) {
        separator += string(CELL_WIDTH, '-') + "+";
    }

    return separator;
}

void BoardFormatter::calculateSides(int totalPlots, int& topCount, int& rightCount, int& bottomCount, int& leftCount) {
    int base = totalPlots / 4;
    int remain = totalPlots % 4;

    topCount = base + (remain > 0 ? 1 : 0);
    rightCount = base + (remain > 1 ? 1 : 0);
    bottomCount = base + (remain > 2 ? 1 : 0);
    leftCount = totalPlots - topCount - rightCount - bottomCount;
}

vector<string> BoardFormatter::makeCenterInfo(const vector<Player>& players, int currentTurn, int maxTurn, int width, int height) {
    vector<string> content;
    content.push_back("==============================================================");
    content.push_back(" _   _ ___ __  __  ___  _   _ ____  ____   ___  _     ___ ");
    content.push_back("| \\ | |_ _|  \\/  |/ _ \\| \\ | / ___||  _ \\ / _ \\| |   |_ _|");
    content.push_back("|  \\| || || |\\/| | | | |  \\| \\___ \\| |_) | | | | |    | | ");
    content.push_back("| |\\  || || |  | | |_| | |\\  |___) |  __/| |_| | |___ | | ");
    content.push_back("|_| \\_|___|_|  |_|\\___/|_| \\_|____/|_|    \\___/|_____|___|");
    content.push_back("==============================================================");
    content.push_back("");
    content.push_back("TURN " + to_string(currentTurn) + " / " + to_string(maxTurn));
    content.push_back("");
    content.push_back("--------------------------------------------------------------");
    content.push_back("LEGENDA KEPEMILIKAN & STATUS");
    content.push_back("P1-P4 : Pemilik properti");
    content.push_back("(1)-(4) : Posisi pemain");
    content.push_back("^ ^^ ^^^ ^^^^: Rumah");
    content.push_back("* : Hotel");
    content.push_back("[M] : Digadaikan");
    content.push_back("");
    content.push_back("PEMAIN:");

    for (int i = 0; i < static_cast<int>(players.size()); i++) {
        if (!players[i].isBankrupt()) {
            content.push_back("P" + to_string(i + 1) + " : " + players[i].getUsername());
        }
    }

    content.push_back("");
    content.push_back("KODE WARNA:");
    content.push_back(
        fgColor(Color::BROWN) + "[CK]" + ansiReset() + " = Coklat     " +
        fgColor(Color::RED) + "[MR]" + ansiReset() + " = Merah"
    );
    content.push_back(
        fgColor(Color::LIGHTBLUE) + "[BM]" + ansiReset() + " = BiruMuda   " +
        fgColor(Color::YELLOW) + "[KN]" + ansiReset() + " = Kuning"
    );
    content.push_back(
        fgColor(Color::PINK) + "[PK]" + ansiReset() + " = Pink       " +
        fgColor(Color::GREEN) + "[HJ]" + ansiReset() + " = Hijau"
    );
    content.push_back(
        fgColor(Color::ORANGE) + "  [OR]" + ansiReset() + " = Orange     " +
        fgColor(Color::DARKBLUE) + "[BT]" + ansiReset() + " = BiruTua"
    );
    content.push_back(
        fgColor(Color::GRAY) + "[AB]" + ansiReset() + " = Utilitas   " +
        "[DF] = Aksi"
    );

    vector<string> result;

    for (int i = 0; i < height; i++) {
        result.push_back(string(width, ' '));
    }

    int start = (height - static_cast<int>(content.size())) / 2;
    if (start < 0) start = 0;

    for (int i = 0; i < static_cast<int>(content.size()); i++) {
        int target = start + i;
        if (target >= height) break;

        result[target] = centerText(content[i], width);
    }

    return result;
}

string BoardFormatter::boardFormat(const Board& board, const vector<Player>& players, int currentTurn, int maxTurn) {
    int totalPlots = board.getSize();

    if (totalPlots == 0) {
        return "[Board] Papan belum diinisialisasi.\n";
    }

    int topCount;
    int rightCount;
    int bottomCount;
    int leftCount;

    calculateSides(totalPlots, topCount, rightCount, bottomCount, leftCount);

    vector<int> topIndexes;
    vector<int> rightIndexes;
    vector<int> bottomIndexes;
    vector<int> leftIndexes;

    for (int i = 0; i < topCount; i++) {
        topIndexes.push_back(i);
    }

    for (int i = 0; i < rightCount; i++) {
        rightIndexes.push_back(topCount + i);
    }

    int bottomStart = topCount + rightCount;

    for (int i = 0; i < bottomCount; i++) {
        bottomIndexes.push_back(bottomStart + bottomCount - 1 - i);
    }

    while (static_cast<int>(bottomIndexes.size()) < topCount) {
        bottomIndexes.push_back(-1);
    }

    for (int i = 0; i < leftCount; i++) {
        leftIndexes.push_back(totalPlots - 1 - i);
    }

    ostringstream oss;

    int middleRows = max(leftCount, rightCount);
    int innerWidth = (topCount - 2) * (CELL_WIDTH + 1) - 1;

    if (innerWidth < 0) {
        innerWidth = 0;
    }

    int centerHeight = middleRows * CELL_HEIGHT;
    vector<string> centerInfo = makeCenterInfo(players, currentTurn, maxTurn, innerWidth, centerHeight);

    auto printHorizontal = [&](const vector<int>& indexes) {
        for (int line = 0; line < CELL_HEIGHT; line++) {
            oss << "|";

            for (int index : indexes) {
                vector<string> cell;

                if (index >= 0) {
                    cell = makeCellLines(index, board, players);
                } else {
                    cell = emptyCell();
                }

                oss << cell[line] << "|";
            }

            oss << "\n";
        }
    };

    oss << makeSeparator(topCount) << "\n";
    printHorizontal(topIndexes);
    oss << makeSeparator(topCount) << "\n";

    int centerLine = 0;

    for (int row = 0; row < middleRows; row++) {
        vector<string> leftCell;
        vector<string> rightCell;

        if (row < static_cast<int>(leftIndexes.size())) {
            leftCell = makeCellLines(leftIndexes[row], board, players);
        } else {
            leftCell = emptyCell();
        }

        if (row < static_cast<int>(rightIndexes.size())) {
            rightCell = makeCellLines(rightIndexes[row], board, players);
        } else {
            rightCell = emptyCell();
        }

        for (int line = 0; line < CELL_HEIGHT; line++) {
            string middle;

            if (centerLine < static_cast<int>(centerInfo.size())) {
                middle = centerInfo[centerLine];
            } else {
                middle = string(innerWidth, ' ');
            }

            oss << "|" << leftCell[line] << "|"
                << middle
                << "|" << rightCell[line] << "|"
                << "\n";

            centerLine++;
        }
    }

    oss << makeSeparator(topCount) << "\n";
    printHorizontal(bottomIndexes);
    oss << makeSeparator(topCount) << "\n";

    return oss.str();
}