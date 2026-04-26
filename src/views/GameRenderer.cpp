#include "views/GameRenderer.hpp"

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
void GameRenderer::showDiceRoll(const Player& player, const Dice& dice, string& postion) {
    cout << Formatter::showDiceRoll(player, dice, postion);
};

void GameRenderer::showControlDice(const Player& player, const Dice& dice, string& position) {
    cout << Formatter::showControlDice(player, dice, position);
};

// ── Deed / Akta (command 4) ──────────────────────────────────────────
void GameRenderer::showDeed(const LandPlot& landPlot) {
    cout << Formatter::makeDeedTable(landPlot);
    
};    

// ── Property list  (CETAK_PROPERTI) (command 5) ──────────────────────────────────────────
 
void GameRenderer::showPropertyList(PlotContext& ctx, const Player& player) {
    cout << Formatter::makePropertyList(ctx, player);
}
 
// ── Buy flow  (BELI) (command 6) ──────────────────────────────────────────────────────────
 
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
 
// ── Build flow  (BANGUN) (commnd 11) ──────────────────────────────────────────────────────
 
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
    cout << Formatter::showLogger(entry);
}

// ── Menang (command 18 - MENANG) ────────────────────────────────────
void GameRenderer::showPlayerList(const Player& player) {
    cout << Formatter::makePlayerList(player);
}

void GameRenderer::showFinalPlayer(string& username) {
    cout << Formatter::finalPlayer(username);
}

void GameRenderer::showWinner(string& username) {
    cout << Formatter::winner(username);
}

// ── Kesempatan dan Dana Umum (command 19 - KESEMPATAN DAN DANA UMUM) ────────────────────────────────────
void GameRenderer::showOnLandChanceCard(ChanceCard& card, string& description) {
    cout << Formatter::chanceCardPlot(card, description);
}

void GameRenderer::showOnLandCommunityChestCard(CommunityChestCard& card, int cost, int playerMoney) {
    cout << Formatter::communityChestPlot(card, cost, playerMoney);
}

// ── Gunakan Kemampuan (command 20 - GUNAKAN KEMAMPUANs) ────────────────────────────────────
void GameRenderer::showCardList(int idx, string& name, string& description) {
    cout << Formatter::makeCardList(idx, name, description);
}

void GameRenderer::showHaveUsedSkillCard(bool isUsed) {
    cout << Formatter::usedSkillCard(isUsed);
}

void GameRenderer::showActivateSkillCard(string& name, string& description) {
    cout << Formatter::activateSkillCard(name, description);
}

void GameRenderer::showEffectSkillCard(string& label, string& description) {
    cout << Formatter::effectSkillCard(label, description);
}

// ── Drop Kartu kemampuan (command 21 - DROP KARTU KEMAMPUAN) ────────────────────────────────────
void GameRenderer::showDropCardWarning(string& name) {
    cout << Formatter::dropCardWarning(name);
}

void GameRenderer::showDropCardAction(string& name) {
    cout << Formatter::dropCardAction(name);
}