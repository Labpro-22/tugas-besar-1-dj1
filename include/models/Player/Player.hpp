#pragma once
#include <functional>
#include <memory>
#include <cstddef>
#include <string>
#include <vector>
#include "models/Player/PlayerStatus.hpp"

class Board;
class Plot;
class PropertyPlot;
class SkillCard;
class SkillContext;
template <class T> class CardDeck;

class Player {
private:
    std::string username;
    int cash;
    int position;
    PlayerStatus status;
    int jailTurns;
    std::vector<std::reference_wrapper<PropertyPlot>> ownedProperties;
    std::vector<std::shared_ptr<SkillCard>> ownedCards;
    bool hasRolled;
    bool shieldActive;
    bool usedSkillThisTurn;
    int shieldTurnLeft;
    int discountTurnLeft;
    int discountValue;
    int consecutiveDoubles; 
    const int JAILDURATIION = 3;

public:
    Player();
    Player(std::string username, int startingCash, int startPosition = 0);

    const std::string& getUsername() const;
    int getCash() const;
    int getPosition() const;
    PlayerStatus getStatus() const;
    int getJailTurns() const;
    const std::vector<std::reference_wrapper<PropertyPlot>>& getOwnedProperties() const;
    const std::vector<std::shared_ptr<SkillCard>>& getOwnedCards() const;
    bool getHasRolled() const;
    bool isShieldActive() const;
    bool hasUsedSkillThisTurn() const;
    int getShieldTurnLeft() const;
    int getDiscountTurnLeft() const;
    int getDiscountValue() const;
    int getConsecutiveDoubles() const;
    int getTotalWealth() const;

    void move();
    void move(int steps, int boardSize = 40); //TODO: tangani untuk kasus dynamic board
    void moveTo(int index, int boardSize = 40);
    void sendToJail(const Board& board);
    void pay(int amount);
    void payTaxes(int amount);
    void payRent(int amount, Player* targetPlayer);
    void buyProperty(PropertyPlot& property);
    void buyProperty(PropertyPlot& property, int price);
    void tradeProperty(PropertyPlot& property, Player* targetPlayer, int price); //TODO: cek apakah aman untuk trade banyak properti sekaligus
    void transferProperty(PropertyPlot& property, Player* targetPlayer);
    void useCards(std::size_t cardIndex, SkillContext& ctx);
    void dropCard(std::size_t cardIndex, CardDeck<std::shared_ptr<SkillCard>>& deck);
    void receive(int amount);

    Player& operator+=(int amount);
    Player& operator-=(int amount);
    bool operator<(Player& other);
    bool operator>(Player& other);

    void setStatus(PlayerStatus newStatus);
    void setJailTurns(int turns);
    void setHasRolled(bool value);
    void setShieldActive(bool value);
    void setShieldTurnLeft(int turns);
    void setDiscountTurnLeft(int turns);
    void setDiscountValue(int value);
    void decrementJailTurns();
    void decrementShieldTurn();
    void decrementDiscountTurn();
    void incrementConsecutiveDoubles();
    void resetConsecutiveDoubles();
    void addOwnedCard(std::shared_ptr<SkillCard> card);
    void addOwnedProperty(PropertyPlot& property);
    void setUsedSkillThisTurn(bool used);
    void resetTurnFlags();

    int countOwnedStation() const;
    int countOwnedUtility() const;

    void updateOwnedProperties();
    void updateStatus();

    void goToJail();

    bool isBankrupt() const;
};
