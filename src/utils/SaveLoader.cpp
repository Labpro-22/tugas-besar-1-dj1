#include "utils/SaveLoader.hpp"
#include <sstream>
using namespace std;

namespace {
int findPlotIndexByCode(const Board& board, const std::string& code) {
    for (int i = 0; i < board.getSize(); ++i) {
        Plot* plot = board.getPlot(i);
        if (plot && plot->getCode() == code) {
            return i;
        }
    }
    return -1;
}
}  // namespace

const string SaveLoader::savePath = "data/";
 
ifstream SaveLoader::open(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        throw FileNotFoundException(path);
    }
    return file;
}
 
bool SaveLoader::fileExists(const string& filename) {
    ifstream file(savePath + filename);
    return file.good();
}
 
PlayerStatus SaveLoader::stringToPlayerStatus(const string& str) {
    if (str.substr(0, 6) == "JAILED") return PlayerStatus::JAILED;
    if (str == "BANKRUPT")            return PlayerStatus::BANKRUPT;
    return PlayerStatus::ACTIVE;
}
 
int SaveLoader::parseJailTurns(const string& str) {
    if (str.size() > 7 && str.substr(0, 7) == "JAILED_") {
        try { return stoi(str.substr(7)); }
        catch (...) { return 0; }
    }
    return 0;
}
 
PropertyStatus SaveLoader::stringToPropertyStatus(const string& str) {
    if (str == "OWNED")     return PropertyStatus::OWNED;
    if (str == "MORTGAGED") return PropertyStatus::MORTGAGED;
    return PropertyStatus::BANK;
}
 
 
shared_ptr<SkillCard> SaveLoader::makeSkillCard(
        const string& name, int value, int duration) {
    (void)duration;
    if (name == "MoveCard") {
        return make_shared<MoveCard>(value);
    }
    if (name == "DiscountCard") {
        return make_shared<DiscountCard>(value);
    }
    if (name == "TeleportCard") {
        return make_shared<TeleportCard>();
    }
    if (name == "ShieldCard") {
        return make_shared<ShieldCard>();
    }
    if (name == "LassoCard") {
        return make_shared<LassoCard>();
    }
    if (name == "DemolitionCard") {
        return make_shared<DemolitionCard>();
    }
    return nullptr;
}
 
map<string,string> SaveLoader::loadPlayerState(ifstream& file, GameState& state) {
    int currentTurn, maxTurn;
    file >> currentTurn >> maxTurn;
    state.setCurrentTurn(currentTurn);
    state.setMaxTurn(maxTurn);
 
    int jumlahPemain;
    file >> jumlahPemain;
    if (jumlahPemain < 2 || jumlahPemain > 4) {
        throw InvalidFileDataException();
    }
 
    map<string, string> playerPosCode;
 
    for (int i = 0; i < jumlahPemain; ++i) {
        string username, posCode, statusStr;
        int cash;
        file >> username >> cash >> posCode >> statusStr;
 
        Player player(username, cash);
        player.setStatus(stringToPlayerStatus(statusStr));
        player.setJailTurns(parseJailTurns(statusStr));
        playerPosCode[username] = posCode;
 
        int jumlahKartu;
        file >> jumlahKartu;
        file.ignore();
 
        for (int j = 0; j < jumlahKartu; ++j) {
            string line;
            getline(file, line);
            istringstream iss(line);
 
            string cardName;
            int value = 0, duration = 0;
            iss >> cardName;
            iss >> value;
            iss >> duration;
 
            auto card = makeSkillCard(cardName, value, duration);
            if (card) {
                if (cardName == "DiscountCard" && duration > 0) {
                    player.setDiscountValue(value);
                    player.setDiscountTurnLeft(duration);
                }
                player.addOwnedCard(std::move(card));
            }
        }
 
        state.addPlayer(player);
    }
 
    string orderLine;
    getline(file, orderLine);
 
    string currentPlayerName;
    file >> currentPlayerName;
 
    const auto& players = state.getPlayers();
    for (int i = 0; i < static_cast<int>(players.size()); ++i) {
        if (players[i].getUsername() == currentPlayerName) {
            state.setCurrentPlayerIdx(i);
            break;
        }
    }
 
    return playerPosCode;
}
 
void SaveLoader::loadPropertyState(ifstream& file, GameState& state, const map<string,string>& playerPosCodes) {
    int jumlahProperti;
    file >> jumlahProperti;
 
    Board& board = state.getBoard();
    auto& players = state.getPlayers();
 
    for (int i = 0; i < jumlahProperti; ++i) {
        string kode, jenis, pemilik, statusStr, nBangunan;
        int fmult, fdur;
        file >> kode >> jenis >> pemilik >> statusStr >> fmult >> fdur >> nBangunan;
 
        int plotIdx = findPlotIndexByCode(board, kode);
        if (plotIdx < 0) continue;
 
        Plot* plot = board.getPlot(plotIdx);
        if (!plot) continue;
 
        PropertyPlot* prop = dynamic_cast<PropertyPlot*>(plot);
        if (!prop) continue;
 
        prop->setOwner(nullptr);
        if (pemilik != "BANK") {
            for (auto& p : players) {
                if (p.getUsername() == pemilik) {
                    prop->setOwner(&p);
                    p.addOwnedProperty(*prop);
                    break;
                }
            }
        }
 
        prop->setPropertyStatus(stringToPropertyStatus(statusStr));
        prop->setFestivalMultiplier(fmult);
        prop->setFestivalDuration(fdur);
 
        LandPlot* land = dynamic_cast<LandPlot*>(prop);
        if (land) {
            int level = (nBangunan == "H") ? 5 : stoi(nBangunan);
            land->setLevel(level);
        }
    }
 
    for (auto& p : players) {
        auto it = playerPosCodes.find(p.getUsername());
        if (it != playerPosCodes.end()) {
            int idx = findPlotIndexByCode(board, it->second);
            if (idx >= 0) {
                p.moveTo(idx, board.getSize());
            }
        }
    }
}
 
void SaveLoader::loadDeckState(ifstream& file, GameState& state) {
    int jumlahKartu;
    file >> jumlahKartu;
    file.ignore();

    vector<shared_ptr<SkillCard>> deckCards;
    for (int i = 0; i < jumlahKartu; ++i) {
        string cardName;
        getline(file, cardName);

        auto card = makeSkillCard(cardName, 0, 0);
        if (card) {
            deckCards.push_back(std::move(card));
        }
    }

    state.getBoard().initializeSkillDeck(std::move(deckCards));
}
 
void SaveLoader::loadLogState(ifstream& file, GameState& state) {
    int jumlahLog;
    file >> jumlahLog;
    file.ignore();
 
    for (int i = 0; i < jumlahLog; ++i) {
        string line;
        getline(file, line);
        istringstream iss(line);
 
        LogEntry entry;
        string detail;
        iss >> entry.turn >> entry.username >> entry.actionType;
 
        getline(iss, detail);
        if (!detail.empty() && detail[0] == ' ') {
            detail = detail.substr(1);
        }
        entry.detail = detail;
 
        state.getLogger().log(entry);
    }
}
 
void SaveLoader::load(GameState& state, const string& filename) {
    ifstream file = open(savePath + filename);
 
    auto posCodes = loadPlayerState(file, state);
    loadPropertyState(file, state, posCodes);
    loadDeckState(file, state);
    loadLogState(file, state);
 
    file.close();
}
