#include <iostream>
#include "core/GameEngine.hpp"
#include "views/Formatter.hpp"
#include "core/BoardFormatter.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
using namespace std;

class GameRenderer {
    public:
        // ── Board (command 1 - CETAK_PAPAN) ─────────────────────────────────
        static void showBoard(const GameState& state);

        // ── Dice (command 2 & 3) ────────────────────────────────────────────
        static void showDiceRoll(const Player& player, const Dice& dice, string& position); // 2
        static void showControlDice(const Player& player, const Dice& dice, string& position); // 3
        
        // ── Deed / Akta (command 4) ──────────────────────────────────────────
        static void showDeed(const LandPlot& landPlot);

        // ── Property list (command 5 - CETAK_PROPERTI) ──────────────────────
        static void showPropertyList(PlotContext& ctx, const Player& player);
 
        // ── Buy flow (command 6 - BELI) ──────────────────────────────────────
        static void showBuyPrompt(const Player& player, const LandPlot& landPlot);
        static void showBuySuccess(const Player& player, const LandPlot& landPlot);
        static void showBuyFailed();
        static void showBuyStation(const StationPlot& station);
        static void showBuyUtility(const UtilityPlot& utility);
 
        // ── Pay rent (command 7 - BAYAR_SEWA) ────────────────────────────────
        static void showPayRent(PlotContext& ctx, const PropertyPlot& property);
        static void showMortgagedPlot(const PropertyPlot& property);
        static void showCannotPayRent(int rentAmount, int playerCash);

        // ── Pay tax (command 8 - BAYAR_PAJAK) ────────────────────────────────
        static void showIncomeTaxPrompt(int flat, int percentage);
        static void showIncomeTaxResult(int totalWealth, int cashBefore, int percentage);
        static void showLuxuryTax(int startMoney, int finalMoney);
        static void showFailPayTax(int money);

        // ── Mortgage (command 9 - GADAI) ──────────────────────────────────────
        static void showCanMortgagedList(const PropertyPlot& property, int money);
        static void showMortgageSuccess(string name, int moneyReceived, int moneyTotal);
        static void showMortgageFailed(const LandPlot& landPlot);
        static void showSellBuildingOption(int idx, const LandPlot& landPlot);
        static void showSellBuildingResult(string& name, int cost);
        
        // ── Redeem (command 10 - TEBUS) ───────────────────────────────────────
        static void showRedeemList(const PropertyPlot& property);
        static void showRedeemNoEligible();
        static void showRedeemSuccess(const PropertyPlot& property, int money);
        static void showRedeemFailed(const PropertyPlot& property, int money);

        // ── Build flow (command 11 - BANGUN) ─────────────────────────────────
        static void showBuildGroupList(const Player& player);
        static void showBuildPlotList(const Player& player, const Color& color);
        static void showBuildSuccess(const Player& player, const LandPlot& landPlot);
        static void showBuildUpgradePrompt(const LandPlot& landPlot);
        static void showBuildUpgradeSuccess(const Player& player, const LandPlot& landPlot);
        static void showBuildNoEligible();

        // ── Auction (command 12 - LELANG) ─────────────────────────────────────
        static void showAuctionStart(const PropertyPlot& property, const string& startingPlayer);
        static void showAuctionTurn(string& username);
        static void showAuctionHighestBid(int amount, const string& bidder);
        static void showAuctionResult(const PropertyPlot& property, string& winner, int cost);

        // ── Festival (command 13 - FESTIVAL) ──────────────────────────────────
        static void showFestivalLanding(const Player& player);
        static void showFestivalApply(int earlyRent, int currRent, int turn);
        static void showFestivalEnhance(int earlyRent, int currRent, int turn);
        static void showFestivalMaximum(int turn);
        static void showFestivalInvalidCode();
        static void showFestivalNotOwned();

        // ── Bankruptcy & liquidation (command 14) ─────────────────────────────
        static void showDebtSummary(int playerMoney, int debt);
        static void showLiquidationPotential(string& status, const PropertyPlot& property);
        static void showMaxPotential(int maxValue);
        static void showLiquidationStatus(int money, int debt);
        static void showSellPropertyList(int idx, const PropertyPlot& property);
        static void showMortgagedList(int idx, const PropertyPlot& property);
        static void showSellPropertyToBank(const PropertyPlot& property, int money);
        static void showPayDebtSuccess(int debt, const Player& visitor, const Player& owner);

        // ── Save / Load (command 15-16) ─────────────────────────────────────
        static void successSaveFile(string& fileName);
        static void failSaveFile(string& fileName);
        static void successLoadFile(string& fileName, string& username);
        static void fileNotFound(string& fileName);
        static void failLoadFile();

        // ── Log (command 17 - CETAK_LOG) ────────────────────────────────────
        static void showLogger(const LogEntry& entry); //17

        // ── Menang (command 18 - MENANG) ────────────────────────────────────
        static void showPlayerList(const Player& player);
        static void showFinalPlayer(string& username);
        static void showWinner(string& username);
        
        // ── Kesempatan dan Dana umum (command 19 - (KESEMPATAN DAN DANA UMUM) ────────────────────────────────────
        static void showOnLandChanceCard(ChanceCard& card, string& description);
        static void showOnLandCommunityChestCard(CommunityChestCard& card, int cost, int playerMoney);

        // ── Gunakan Kemampuan (command 20 - GUNAKAN KEMAMPUAN) ────────────────────────────────────
        static void showCardList(int idx, string& name, string& description);
        static void showHaveUsedSkillCard(bool isUsed);
        static void showActivateSkillCard(string& name, string& description);
        static void showEffectSkillCard(string& label, string& desciption);

        // ── Drop card (command 21 - DROP KARTU KEMAMPUAN) ────────────────────────────────────
        static void showDropCardWarning(string& name);
        static void showDropCardAction(string& name);
};
