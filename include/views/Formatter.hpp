#pragma once
#include "core/GameEngine.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "models/Plot/PropertyPlot/StationPlot.hpp"
#include "models/Plot/PropertyPlot/UtilityPlot.hpp"
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
#include "sstream"
#include <iomanip>

class Formatter {
    public:
    // ── Primitives ───────────────────────────────────────────────────────
        static string moneyString(int value);
        static string colorString(const Color& color);
        static string statusString(const PropertyStatus& status);
        static string onLand(string name, string description);
        static string playerMoney(int money);

    // ── Dice (command 2 & 3) ─────────────────────────────────────────────
        static string showDiceRoll(const Player& player, const Dice& dice, string& position);
        static string showControlDice(const Player& player, const Dice& dice, string& position); 
        
    // ── Deed (command 4 - CETAK_AKTA) ────────────────────────────────────
        static string makeDeedTable(const LandPlot& landPlot);
        static string deedNotFound(const string& code);                                                 //TAMBAHAN
        
    // ── Property list (command 5 - CETAK_PROPERTI) ───────────────────────
        static string makePropertyList(const PropertyPlot& property);
        static string makePropertyList(const PropertyPlot& property, const Color& color);
        static string makePropertyList(PlotContext& ctx, const Player& player);

    // ── Buy flow (command 6 - BELI) ──────────────────────────────────────
        static string buyProperty(const Player& player, const LandPlot& landPlot);
        static string buySuccess(const Player& player, const LandPlot& landPlot);
        static string buyFailed();
        static string buyStation(const StationPlot& station);
        static string buyUtility(const UtilityPlot& utility);

    // ── Pay rent (command 7 - BAYAR_SEWA) ────────────────────────────────
        static string makePayRent(PlotContext& ctx, const PropertyPlot& property); // TODO: calculate pakai const
        static string mortgagedPlot(const PropertyPlot& property); 
        static string cannotPayRent(int rentAmount, int playerCash);                                   //TAMBAHAN
        
    // ── Pay tax (command 8 - BAYAR_PAJAK) ────────────────────────────────────────
        static string payIncomeTax(int flat, int percentage);
        static string payIncomeTaxResult(int totalWealth, int cashBefore, int percentage);              //TAMBAHAN
        static string payFlatTax(int flat, int money);
        static string failPayFlatTax(int flat, int money);
        static string payLuxuryTax(int startMoney, int finalMoney);
        static string failPayLuxuryTax(int money);
   
    // ── Mortgage (command 9 - GADAI [TODO REVISION]) ────────────────────────────────────── 
        static string makeCanMortgagedList(const PropertyPlot& property, int money);
        static string successMortgage(string name, int moneyRecieved, int moneyTotal); // TODO: Revisi nanti
        static string failedMortgage(const LandPlot& landplot); 
        static string makeSellBuildingOption(int idx, const LandPlot& landPlot); 
        static string sellProperty(string& name, int cost);

    // ── Redeem (command 10 - TEBUS) ───────────────────────────────────────────────
        static string makeRedeemList(const PropertyPlot& property);
        static string redeemNoEligible();                                                                //TAMBAHAN
        static string successRedeemProperty(const PropertyPlot& property, int money);
        static string failedRedeemProperty(const PropertyPlot& property, int money);

    // ── Build flow (command 11 - BANGUN) ─────────────────────────────────────────
        static string buildGroupList(const Player& player);
        static string buildPlotList(const Player& player, const Color& color);
        static string buildSuccess(const Player& player, const LandPlot& landPlot);
        static string buildUpgradePrompt(const LandPlot& landPlot);
        static string buildUpgradeSuccess(const Player& player, const LandPlot& landPlot);
        static string buildNoEligible();

    // ── Auction (command 12 - LELANG) ─────────────────────────────────────
        static string auctionStart(const PropertyPlot& property, const string& startingPlayer);            //TAMBAHAN
        static string auctionTurn(string& username);
        static string auctionHighestBid(int amount, const string& bidder);                                 //TAMBAHAN
        static string auctionResult(const PropertyPlot& property, string& username, int cost);

    // ── Festival (command 13 - FESTIVAL) ──────────────────────────────────
        static string festivalLanding(const Player& player);                                               //TAMBAHAN
        static string applyFestival(int earlyRent, int currRent, int turn);
        static string enhanceFestival(int earlyRent, int currRent, int turn);
        static string maximumFestival(int turn);
        static string festivalInvalidCode();                                                               //TAMBAHAN
        static string festivalNotOwned();                                                                  //TAMBAHAN

    // ── Bankruptcy & liquidation (command 14 [TODO REVISION]) ─────────────────────────────
        static string makeDebtSummary(int playerMoney, int debt);
        static string potentialLiquidation(string& status, const PropertyPlot& property);
        static string maxPotentialValue(int maxValue);
        static string makeLiquidationStatus(int playerMoney, int debt);
        static string sellPropertyList(int idx, const PropertyPlot& property);
        static string mortgagedList(int idx, const PropertyPlot& property);
        static string sellPropertyToBank(const PropertyPlot& property, int money);
        static string payDebtSuccess(int debt, const Player& visitor, const Player& owner);

    // ── Save / Load (command 15-16) ───────────────────────────────────────
        static string successSaveFile(string& fileName);
        static string failSaveFile(string& fileName);
        static string successLoadFile(string& fileName, string& username);
        static string fileNotFound(string& fileName);
        static string failLoadFile();

    // ── Log (command 17 - CETAK_LOG) ─────────────────────────────────────
        static string showLogger(const LogEntry& log);

    // ── Win (command 18 - MENANG) ─────────────────────────────────────────
        static string makePlayerList(const Player& player);
        static string winner(string username);
        static string finalPlayer(string username);

    // ── Chance / Community Chest (command 19) ─────────────────────────────
        static string chanceCardPlot(ChanceCard& card, string& description);
        static string communityChestPlot(CommunityChestCard& card, int cost, int playerMoney);
    
    // ── Skill cards (command 20 - GUNAKAN_KEMAMPUAN) ─────────────────────
        static string makeCardList(int idx, string& name, string& description);
        static string usedSkillCard(bool isUsed);
        static string activateSkillCard(string& name, string& description);
        static string effectSkillCard(string& label, string& description);

    // ── Drop card (command 21 - DROP KARTU) ──────────────────────────────
        static string dropCardWarning(string& name);
        static string dropCardAction(string& name);

    // ── Special plots ─────────────────────────────────────────────────
        static string landOnGo();
        static string landOnFreePark();
        static string landOnGoPrison();
        static string landOnPrisonVisit();
        static string landOnPrisonJailed(int jailTurnsLeft);

    // ── Exception ──────────────────────────────────────────────────────────────
        static string throwException(const GameException e);
};

