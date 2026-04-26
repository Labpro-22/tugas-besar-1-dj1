#pragma once
#include <vector>
#include <string>
#include <memory>

#include "CardDeck.hpp"
#include "models/Plot/Plot.hpp"
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
#include "models/Card/SkillCard/SkillCard.hpp"

class Board {
public:
    static const int MIN_BOARD_SIZE = 20;
    static const int MAX_BOARD_SIZE = 60;
    static const int DEFAULT_BOARD_SIZE = 40;
 
private:
    vector<unique_ptr<Plot>> tiles;
 
    CardDeck<std::unique_ptr<ChanceCard>>         chanceDeckPile;
    CardDeck<std::unique_ptr<CommunityChestCard>> communityChestDeckPile;
    CardDeck<std::shared_ptr<SkillCard>>          skillCardDeckPile;

    vector<std::unique_ptr<ChanceCard>> ownedChanceCards;
    vector<std::unique_ptr<CommunityChestCard>> ownedCommunityChestCards;
    vector<std::shared_ptr<SkillCard>> ownedSkillCards;
 
    Board(const Board&)            = delete;
    Board& operator=(const Board&) = delete;
 
public:
    Board();
    ~Board() = default;
 
    Board(Board&&) noexcept            = default;
    Board& operator=(Board&&) noexcept = default;
 
    bool addPlot(unique_ptr<Plot> plot);
    bool isReady() const;
 
    Plot* getPlot(int index) const;
    Plot* getPlotByCode(const std::string& code) const;
    int getSize() const;
    const vector<unique_ptr<Plot>>& getPlots() const;
   const vector<Plot*> getPlots(Color color) const;
    
    int findPlotIndex(const PlotType type) const;
    PlotType getPlotType(int idx) const;
    bool isIndexValid(int idx) const;
    bool isPropertyPlot(int idx) const;
    bool isPlayerOwnAllColor(Color color, const Player* player) const;

    void initializeChanceDeck(std::vector<unique_ptr<ChanceCard>> cards);
    void initializeCommunityChestDeck(std::vector<unique_ptr<CommunityChestCard>> cards);
    void initializeSkillDeck(std::vector<std::shared_ptr<SkillCard>> cards);
 
    CardDeck<std::unique_ptr<ChanceCard>>& getChanceDeckPile();
    CardDeck<std::unique_ptr<CommunityChestCard>>& getCommunityChestDeckPile();
    CardDeck<std::shared_ptr<SkillCard>>& getSkillCardDeckPile();
 
    const CardDeck<std::unique_ptr<ChanceCard>>& getChanceDeckPile() const;
    const CardDeck<std::unique_ptr<CommunityChestCard>>& getCommunityChestDeckPile() const;
    const CardDeck<std::shared_ptr<SkillCard>>& getSkillCardDeckPile() const;
 
    std::unique_ptr<ChanceCard> drawChanceCard();
    std::unique_ptr<CommunityChestCard> drawCommunityChestCard();
    std::shared_ptr<SkillCard> drawSkillCard();
};
