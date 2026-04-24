#include "core/GameEngine.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
#include "sstream"

class Formatter {
    public:
        static string moneyString(int value);
        static string colorString(const Color& color);
        static string statusString(const PropertyStatus& status);

        static string onLand(string name, string description);
        static string playerMoney(int money);

        static string showDiceRoll(string username, const Dice& dice, string position);
        static string showControlDice(string username, const Dice& dice, string position); 
        static string makeDeedTable(const LandPlot& landPlot);

        static string makePropertyList(string name, string code);
        static string makePropertyList(Color& color, string name, string code); // TODO:

        static string makeMortgagedList(string name, string code, Color& color, int money);
        static string successRedeemProperty(string name, int cost, int playerMoney);
        static string failedRedeemProperty(string name, int cost, int playerMoney);

        static string auctionTurn(string username);
        static string auctionResult(string name, int cost, string plotName, string code);

        static string makeCanMortgagedList(string name, string code, Color& color, int money);
        static string successMortgage(string name, int moneyRecieved, int moneyTotal);
        static string failedMortgage(const LandPlot& landplot); //TODO:
        static string sellProperty(string name, int cost);
        
        static string makePayRent(const Player& visitor, const Player& owner, int level, int rentValue, string code); 
        static string mortgagedPlot(string ownerName, string plotName, string code);

        static string payIncomeTax();
        static string payLuxuryTax(int startMoney, int finalMoney);
        static string makeWealthList(const Player& player);  // TODO: Total kekayaan di property dan bangunan

        static string applyFestival(int startMoney, int finalMoney, int turn);
        static string enhanceFestival(int startMoney, int finalMoney, int turn);
        static string maximumFestival(int turn);

        static string makePlayerList(string name, int money, int totalProperty, int totalCard);
        static string winner(string username);
        static string finalPlayer(string username);

        static string communityChestPlot(CommunityChestCard& card, int cost, int playerMoney);

        static string showLogger(int turn, string username, string actionType, string detail);
        
        static string makeCardList(int idx, string name, string description);
        static string usedSkillCard(bool isUsed);
        static string activateSkillCard(string name, string description);
        static string effectSkillCard(string label, string description);

        static string dropCardWarning(string name);
        static string dropCardAction(string name);


        static string throwException(GameException e);
};

