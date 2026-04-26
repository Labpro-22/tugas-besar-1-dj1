#include "utils/ConfigSaver.hpp"
using namespace std; 

const string ConfigSaver::savePath            = "data/";
const string ConfigSaver::defaultSaveFileName = "save.txt";
 
ofstream ConfigSaver::open(const string& path) {
    ofstream file(path);
    if (!file.is_open()) {
        throw FileNotFoundException(path);
    }
    return file;
}
 
bool ConfigSaver::fileExists(const string& filename) {
    ifstream file(savePath + filename);
    return file.good();
}
 
std::string ConfigSaver::playerStatusToString(PlayerStatus status, int jailTurns) {
    switch (status) {
        case PlayerStatus::ACTIVE:   return "ACTIVE";
        case PlayerStatus::JAILED:   return "JAILED_" + to_string(jailTurns);
        case PlayerStatus::BANKRUPT: return "BANKRUPT";
        default:                     return "ACTIVE";
    }
}
 
string ConfigSaver::propertyStatusToString(PropertyStatus status) {
    switch (status) {
        case PropertyStatus::BANK:      return "BANK";
        case PropertyStatus::OWNED:     return "OWNED";
        case PropertyStatus::MORTGAGED: return "MORTGAGED";
        default:                        return "BANK";
    }
}
 
string ConfigSaver::plotTypeToString(PlotType type) {
    switch (type) {
        case PlotType::LANDPLOT:    return "street";
        case PlotType::STATIONPLOT: return "railroad";
        case PlotType::UTILITYPLOT: return "utility";
        default:                    return "street";
    }
}
 
string ConfigSaver::cardValueString(const shared_ptr<SkillCard>& card,
                                     const Player& player) {
    
    MoveCard* move = dynamic_cast<MoveCard*>(card.get());
    if (move) {
        return to_string(move->getMoveNumber());
    }
 
    DiscountCard* discount = dynamic_cast<DiscountCard*>(card.get());
    if (discount) {
    return to_string(discount->getDiscountValue()) + " "
         + to_string(player.getDiscountTurnLeft());
    }
 
    TeleportCard* teleport = dynamic_cast<TeleportCard*>(card.get());
    if (teleport) {
        return to_string(teleport->getTargetIndex());
    }
 
    return "";
}
 
void ConfigSaver::savePlayerState(ofstream& file, const GameState& state) {
    const auto& players = state.getPlayers();
    const Board& board  = state.getBoard();
 
    file << state.getCurrentTurn() << " " << state.getMaxTurn() << "\n";
 
    file << players.size() << "\n";
 
    for (const Player& p : players) {
        
        Plot* currentPlot = board.getPlot(p.getPosition());
        string posCode = currentPlot ? currentPlot->getCode() : "GO";
 
        file << p.getUsername() << " "
             << p.getCash()     << " "
             << posCode         << " "
             << playerStatusToString(p.getStatus(), p.getJailTurns()) << "\n";
 
        const auto& cards = p.getOwnedCards();
        file << cards.size() << "\n";
 
        for (const auto& card : cards) {
            string valueStr = cardValueString(card, p);
            file << card->getName();
            if (!valueStr.empty()) file << " " << valueStr;
            file << "\n";
        }
    }
 
    for (int i = 0; i < static_cast<int>(players.size()); ++i) {
        if (i > 0) file << " ";
        file << players[i].getUsername();
    }
    file << "\n";
 
    file << state.getCurrentPlayer().getUsername() << "\n";
}
 
void ConfigSaver::savePropertyState(ofstream& file, const GameState& state) {
    const auto& plots = state.getBoard().getPlots();
 
    int count = 0;
    for (const auto& plotPtr : plots) {
        if (dynamic_cast<PropertyPlot*>(plotPtr.get())) count++;
    }
 
    file << count << "\n";
 
    for (const auto& plotPtr : plots) {
        PropertyPlot* prop = dynamic_cast<PropertyPlot*>(plotPtr.get());
        if (!prop) continue;
 
        string jenis   = plotTypeToString(prop->getType());
        string pemilik = prop->getOwner()
                       ? prop->getOwner()->getUsername()
                       : "BANK";
        string status  = propertyStatusToString(prop->getPropertyStatus());
        int fmult      = prop->getFestivalMultiplier();  
        int fdur       = prop->getFestivalDuration();    
 
        string nBangunan = "0";
        LandPlot* land = dynamic_cast<LandPlot*>(prop);
        if (land) {
            int level = land->getLevel();
            nBangunan = (level == 5) ? "H" : to_string(level);
        }
 
        file << prop->getCode() << " "
             << jenis           << " "
             << pemilik         << " "
             << status          << " "
             << fmult           << " "
             << fdur            << " "
             << nBangunan       << "\n";
    }
}
 
void ConfigSaver::saveDeckState(ofstream& file, const GameState& state) {
    
    const auto& deckCards = state.getBoard()
                                 .getSkillCardDeckPile()
                                 .getAllCards();
 
    file << deckCards.size() << "\n";
 
    for (const auto& card : deckCards) {
        
        file << card->getName() << "\n";
    }
}
 
void ConfigSaver::saveLogState(ofstream& file, const GameState& state) {
    const auto& logs = state.getLogs();
 
    file << logs.size() << "\n";
 
    for (const LogEntry& entry : logs) {
        
        file << entry.turn       << " "
             << entry.username   << " "
             << entry.actionType << " "
             << entry.detail     << "\n";
    }
}
 
void ConfigSaver::save(const GameState& state, const string& filename) {
    ofstream file = open(savePath + filename);
 
    savePlayerState(file, state);
    savePropertyState(file, state);
    saveDeckState(file, state);
    saveLogState(file, state);
 
    file.close();
}