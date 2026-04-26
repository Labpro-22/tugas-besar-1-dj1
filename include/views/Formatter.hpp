#include "core/GameEngine.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "models/Plot/PropertyPlot/StationPlot.hpp"
#include "models/Plot/PropertyPlot/UtilityPlot.hpp"
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
#include "sstream"
#include <iomanip>

class Formatter {
    public:
        static string moneyString(int value);
        static string colorString(const Color& color);
        static string statusString(const PropertyStatus& status);

        static string onLand(string name, string description);
        static string playerMoney(int money);

        static string showDiceRoll(const Player& player, const Dice& dice, string& position);
        static string showControlDice(const Player& player, const Dice& dice, string& position); 
        static string makeDeedTable(const LandPlot& landPlot);

        static string makePropertyList(const PropertyPlot& property);
        static string makePropertyList(const PropertyPlot& property, const Color& color);
        static string makePropertyList(PlotContext& ctx, const Player& player);

        static string buyProperty(const Player& player, const LandPlot& landPlot);
        static string buySuccess(const Player& player, const LandPlot& landPlot);
        static string buyFailed();
        static string buyStation(const StationPlot& station);
        static string buyUtility(const UtilityPlot& utility);

        static string buildGroupList(const Player& player);
        static string buildPlotList(const Player& player, const Color& color);
        static string buildSuccess(const Player& player, const LandPlot& landPlot);
        static string buildUpgradePrompt(const LandPlot& landPlot);
        static string buildUpgradeSuccess(const Player& player, const LandPlot& landPlot);
        static string buildNoEligible();

        static string makeRedeemList(const PropertyPlot& property);
        static string successRedeemProperty(const PropertyPlot& property, int money);
        static string failedRedeemProperty(const PropertyPlot& property, int money);

        static string auctionTurn(string& username);
        static string auctionResult(const PropertyPlot& property, string& username, int cost);

        static string makeCanMortgagedList(const PropertyPlot& property, int money);
        static string successMortgage(string name, int moneyRecieved, int moneyTotal); // TODO: Revisi nanti
        static string failedMortgage(const LandPlot& landplot); 
        static string makeSellBuildingOption(int idx, const LandPlot& landPlot); 
        static string sellProperty(string& name, int cost);
        
        static string makePayRent(PlotContext& ctx, const PropertyPlot& property); // TODO: calculate pakai const
        static string mortgagedPlot(const PropertyPlot& property); 

        static string payIncomeTax(int flat, int percentage);
        static string payLuxuryTax(int startMoney, int finalMoney);
        static string failPayTax(int money);

        static string applyFestival(int earlyRent, int currRent, int turn);
        static string enhanceFestival(int earlyRent, int currRent, int turn);
        static string maximumFestival(int turn);

        static string makePlayerList(const Player& player);
        static string winner(string username);
        static string finalPlayer(string username);

        static string chanceCardPlot(ChanceCard& card, string& description);
        static string communityChestPlot(CommunityChestCard& card, int cost, int playerMoney);

        static string showLogger(const LogEntry& log);
        
        static string makeCardList(int idx, string& name, string& description);
        static string usedSkillCard(bool isUsed);
        static string activateSkillCard(string& name, string& description);
        static string effectSkillCard(string& label, string& description);

        static string makeDebtSummary(int playerMoney, int debt);
        static string potentialLiquidation(string& status, const PropertyPlot& property);
        static string maxPotentialValue(int maxValue);
        static string makeLiquidationStatus(int playerMoney, int debt);
        static string sellPropertyList(int idx, const PropertyPlot& property);
        static string mortgagedList(int idx, const PropertyPlot& property);
        static string sellPropertyToBank(const PropertyPlot& property, int money);
        static string payDebtSuccess(int debt, const Player& visitor, const Player& owner);

        static string dropCardWarning(string& name);
        static string dropCardAction(string& name);

        static string successSaveFile(string& fileName);
        static string failSaveFile(string& fileName);

        static string successLoadFile(string& fileName, string& username);
        static string fileNotFound(string& fileName);
        static string failLoadFile();

        static string throwException(const GameException e);
};

