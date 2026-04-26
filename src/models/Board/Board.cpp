#include "models/Board/Board.hpp"
 
Board::Board()
    : tiles(),
      chanceDeckPile(),
      communityChestDeckPile(),
      skillCardDeckPile()
{
    tiles.reserve(DEFAULT_BOARD_SIZE);
}
 
bool Board::addPlot(unique_ptr<Plot> plot) {
    if (!plot) return false;
    if (static_cast<int>(tiles.size()) >= MAX_BOARD_SIZE) {
        cerr << "[Board] Papan sudah penuh (" << MAX_BOARD_SIZE
                  << " petak). Plot \"" << plot->getCode()
                  << "\" tidak ditambahkan." << endl;
        return false;
    }
    tiles.push_back(move(plot));
    return true;
}
 
bool Board::isReady() const {
    int size = static_cast<int>(tiles.size());
    return size >= MIN_BOARD_SIZE && size <= MAX_BOARD_SIZE;
}
 
Plot* Board::getPlot(int index) const {
    if (index < 0 || index >= static_cast<int>(tiles.size())) {
        return nullptr;
    }
    return tiles[index].get(); 
}

Plot* Board::getPlotByCode(const std::string& code) const {
    for (const auto& plot : tiles) {
        if (plot->getCode() == code) {
            return plot.get();
        }
    }
    return nullptr;
}
 
int Board::getSize() const {
    return static_cast<int>(tiles.size());
}
 
const vector<unique_ptr<Plot>>& Board::getPlots() const {
    return tiles;
}

const vector<Plot*> Board::getPlots(Color color) const {
    std::vector<Plot*> result;
    for (const auto& p : tiles) {
        if (p->getColor() == color) {
            result.push_back(p.get());
        }
    }
    return result;
}

int Board::findPlotIndex(const PlotType type) const {
    for (int i = 0; i < tiles.size(); ++i) {
        if (tiles[i]->getType() == type) {
            return i;
        }
    }
    return -1;
}

PlotType Board::getPlotType(int idx) const {
    if (!isIndexValid(idx)) throw PositionNotInBoardException(idx);
    return tiles[idx]->getType();
}

bool Board::isIndexValid(int idx) const {
    if (idx <= 0 || idx > getSize()){
        return false;   
    }
    return true;
}

bool Board::isPropertyPlot(int idx) const{
    PlotType type = getPlotType(idx);
    if (type == PlotType::LANDPLOT ||
        type == PlotType::STATIONPLOT ||
        type == PlotType::UTILITYPLOT){
        return true;
    }
    return false;
}

bool Board::isPlayerOwnAllColor(Color color, const Player* player) const {
    for (const auto& plot : tiles) {
        if (plot->getColor() == color && plot->getOwner() != player){
            return false;   
        }
    }
    return true;
}

void Board::initializeChanceDeck(std::vector<std::unique_ptr<ChanceCard>> cards) {
    chanceDeckPile.initialize(std::move(cards));
    chanceDeckPile.shuffle();
}

void Board::initializeCommunityChestDeck(std::vector<std::unique_ptr<CommunityChestCard>> cards) {
    communityChestDeckPile.initialize(std::move(cards));
    communityChestDeckPile.shuffle();
}

void Board::initializeSkillDeck(std::vector<std::shared_ptr<SkillCard>> cards) {
    skillCardDeckPile.initialize(std::move(cards));
    skillCardDeckPile.shuffle();
}

CardDeck<std::unique_ptr<ChanceCard>>& Board::getChanceDeckPile() {
    return chanceDeckPile;
}
CardDeck<std::unique_ptr<CommunityChestCard>>& Board::getCommunityChestDeckPile() {
    return communityChestDeckPile;
}
CardDeck<std::shared_ptr<SkillCard>>& Board::getSkillCardDeckPile() {
    return skillCardDeckPile;
}
 
const CardDeck<std::unique_ptr<ChanceCard>>& Board::getChanceDeckPile() const {
    return chanceDeckPile;
}
const CardDeck<std::unique_ptr<CommunityChestCard>>& Board::getCommunityChestDeckPile() const {
    return communityChestDeckPile;
}
const CardDeck<std::shared_ptr<SkillCard>>& Board::getSkillCardDeckPile() const {
    return skillCardDeckPile;
}
 
std::unique_ptr<ChanceCard> Board::drawChanceCard() {
    return chanceDeckPile.draw();
}
 
std::unique_ptr<CommunityChestCard> Board::drawCommunityChestCard() {
    return communityChestDeckPile.draw();
}
 
std::shared_ptr<SkillCard> Board::drawSkillCard() {
    return skillCardDeckPile.draw();
}

