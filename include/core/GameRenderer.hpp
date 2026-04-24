#include "iostream"
#include "core/GameEngine.hpp"
#include "core/Formatter.hpp"
#include "core/BoardFormatter.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
using namespace std;

class GameRenderer {
    public:
        // ── Board (command 1 - CETAK_PAPAN) ─────────────────────────────────
        static void showBoard(const GameState& state);

        // ── Dice (command 2 & 3) ────────────────────────────────────────────
        static void showDiceRoll(string username, const Dice& dice, string position); // 2
        static void showControlDice(string username, const Dice& dice, string position); // 3
        
        // ── Deed / Akta (command 4) ──────────────────────────────────────────
        static void showDeed(PropertyPlot& property);

        // ── Property list (command 5 - CETAK_PROPERTI) ──────────────────────
        // Shows the full owned-property list for the active player
        static void showPropertyList(PlotContext& ctx, const Player& player);
 
        // ── Buy flow (command 6 - BELI) ──────────────────────────────────────
        // Prompt shown when player lands on an unowned LandPlot
        static void showBuyPrompt(const Player& player, const LandPlot& landPlot);
        // Confirmation after player says "y"
        static void showBuySuccess(const Player& player, const LandPlot& landPlot);
        // Shown when player says "n" or cannot afford (triggers auction)
        static void showBuyFailed();
        // Automatic acquisition of a station
        static void showBuyStation(const StationPlot& station);
        // Automatic acquisition of a utility
        static void showBuyUtility(const UtilityPlot& utility);
 
        // ── Build flow (command 11 - BANGUN) ─────────────────────────────────
        // Step 1: list color groups the player can build on
        static void showBuildGroupList(const Player& player);
        // Step 2: list individual plots inside the chosen color group
        static void showBuildPlotList(const Player& player, const Color& color);
        // Confirmation after a house is built
        static void showBuildSuccess(const Player& player, const LandPlot& landPlot);
        // Prompt shown when a plot already has 4 houses (hotel upgrade)
        static void showBuildUpgradePrompt(const LandPlot& landPlot);
        // Confirmation after hotel upgrade
        static void showBuildUpgradeSuccess(const Player& player, const LandPlot& landPlot);
        // Shown when the player owns no eligible color group
        static void showBuildNoEligible();

        // ── Festival (command 13) ────────────────────────────────────────────
        static void showFestival(); 

        // ── Save / Load (command 15-16) ─────────────────────────────────────
        static void showSaveSuccess(string filename); 
        static void showSaveFailed(string filename); 


        // ── Log (command 17 - CETAK_LOG) ────────────────────────────────────
        static void showLogger(const LogEntry& entry); //17
};