#include "core/GameRenderer.hpp"

// ── Board (command 1 - CETAK_PAPAN) ─────────────────────────────────
void GameRenderer::showBoard(const GameState& state) {
    cout << BoardFormatter::boardFormat(
        state.getBoard(),
        state.getPlayers(),
        state.getCurrentTurn(),
        state.getMaxTurn()
    );
};

// ── Dice (command 2 & 3) ────────────────────────────────────────────
void GameRenderer::showDiceRoll(string username, const Dice& dice, string position) {
    cout << "Mengocok dadu..." << endl;
    showControlDice(username, dice, position);
};

void GameRenderer::showControlDice(string username, const Dice& dice, string position) {
    cout << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    cout << "Memajukan Bidak " << username << " sebanyak " << dice.getTotal() << "petak..." << endl;
    cout << username << " mendarat di " << position << endl;
};

// ── Deed / Akta (command 4) ──────────────────────────────────────────
void GameRenderer::showDeed(PropertyPlot& property) {
    cout << "+===========================================+" << endl;
    cout << "|              AKTA KEPEMILIKAN             |" << endl;
    cout << "| " << property.getName() << " (" << property.getCode() << ")" << " |" << endl;
    cout << "+===========================================+" << endl;
    
};    

// ── Property list  (CETAK_PROPERTI) ──────────────────────────────────────────
 
void GameRenderer::showPropertyList(PlotContext& ctx, const Player& player) {
    cout << Formatter::makePropertyList(ctx, player);
}
 
// ── Buy flow  (BELI) ──────────────────────────────────────────────────────────
 
void GameRenderer::showBuyPrompt(const Player& player, const LandPlot& landPlot) {
    cout << Formatter::buyProperty(player, landPlot);
}
 
void GameRenderer::showBuySuccess(const Player& player, const LandPlot& landPlot) {
    cout << Formatter::buySuccess(player, landPlot);
}
 
void GameRenderer::showBuyFailed() {
    cout << Formatter::buyFailed();
}
 
void GameRenderer::showBuyStation(const StationPlot& station) {
    cout << Formatter::buyStation(station);
}
 
void GameRenderer::showBuyUtility(const UtilityPlot& utility) {
    cout << Formatter::buyUtility(utility);
}
 
// ── Build flow  (BANGUN) ──────────────────────────────────────────────────────
 
void GameRenderer::showBuildGroupList(const Player& player) {
    cout << Formatter::buildGroupList(player);
}
 
void GameRenderer::showBuildPlotList(const Player& player, const Color& color) {
    cout << Formatter::buildPlotList(player, color);
}
 
void GameRenderer::showBuildSuccess(const Player& player, const LandPlot& landPlot) {
    cout << Formatter::buildSuccess(player, landPlot);
}
 
void GameRenderer::showPropertyList(PlotContext& ctx, const Player& player) {
    cout << Formatter::makePropertyList(ctx, player);
}
 
void GameRenderer::showBuildUpgradeSuccess(const Player& player, const LandPlot& landPlot) {
    cout << Formatter::buildUpgradeSuccess(player, landPlot);
}
 
void GameRenderer::showBuildNoEligible() {
    cout << Formatter::buildNoEligible();
}
 

// ── Save / Load (command 15-16) ─────────────────────────────────────
void GameRenderer::showSaveSuccess(string filename) {
    cout << "Menyimpan permainan..." << endl;
    cout << "Permainan berhasil disimpan ke: " << filename << endl;
};

void GameRenderer::showSaveFailed(string filename) {
    cout << "Gagal menyimpan file! pastikan direktori dapat ditulis." << endl;
}

// ── Log (command 17 - CETAK_LOG) ────────────────────────────────────
void GameRenderer::showLogger(const LogEntry& entry){
    cout << "[" << entry.turn << "] " << entry.username << " | " << entry.actionType << " | " << entry.detail << endl; 
}
