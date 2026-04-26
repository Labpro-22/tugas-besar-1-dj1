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

// ── Pay rent (command 7 - BAYAR_SEWA) ────────────────────────────────
void GameRenderer::showPayRent(PlotContext& ctx, const PropertyPlot& property) {
    cout << Formatter::makePayRent(ctx, property);
}

void GameRenderer::showMortgagedPlot(const PropertyPlot& property) {
    cout << Formatter::mortgagedPlot(property);
}

void GameRenderer::showCannotPayRent(int rentAmount, int playerCash) {
    cout << Formatter::cannotPayRent(rentAmount, playerCash);
}

// ── Pay tax (command 8 - BAYAR_PAJAK) ────────────────────────────────
void GameRenderer::showIncomeTaxPrompt(int flat, int percentage) {
    cout << Formatter::payIncomeTax(flat, percentage);
}

void GameRenderer::showIncomeTaxResult(int totalWealth, int cashBefore, int percentage) {
    cout << Formatter::payIncomeTaxResult(totalWealth, cashBefore, percentage);
}

void GameRenderer::showLuxuryTax(int startMoney, int finalMoney) {
    cout << Formatter::payLuxuryTax(startMoney, finalMoney);
}

void GameRenderer::showFailPayTax(int money) {
    cout << Formatter::failPayTax(money);
}

// ── Mortgage (command 9 - GADAI) ──────────────────────────────────────
void GameRenderer::showCanMortgagedList(const PropertyPlot& property, int money){
    cout << Formatter::makeCanMortgagedList(property, money);
}

void GameRenderer::showMortgageSuccess(string name, int moneyReceived, int moneyTotal) {
    cout << Formatter::successMortgage(name, moneyReceived, moneyTotal);
}

void GameRenderer::showMortgageFailed(const LandPlot& landPlot) {
    cout << Formatter::failedMortgage(landPlot);
}

void GameRenderer::showSellBuildingOption(int idx, const LandPlot& landPlot) {
    cout << Formatter::makeSellBuildingOption(idx, landPlot);
}

void GameRenderer::showSellBuildingResult(string& name, int cost) {
    cout << Formatter::sellProperty(name, cost);
}
 
// ── Redeem (command 10 - TEBUS) ───────────────────────────────────────
void GameRenderer::showRedeemList(const PropertyPlot& property) {
    cout << Formatter::makeRedeemList(property);
}

void GameRenderer::showRedeemNoEligible() {
    cout << Formatter::redeemNoEligible();
}

void GameRenderer::showRedeemSuccess(const PropertyPlot& property, int money) {
    cout << Formatter::successRedeemProperty(property, money);
}

void GameRenderer::showRedeemFailed(const PropertyPlot& property, int money) {
    cout << Formatter::failedRedeemProperty(property, money);
}

// ── Build flow  (BANGUN) (command 11) ──────────────────────────────────────────────────────
void GameRenderer::showBuildGroupList(const Player& player) {
    cout << Formatter::buildGroupList(player);
}
 
void GameRenderer::showBuildPlotList(const Player& player, const Color& color) {
    cout << Formatter::buildPlotList(player, color);
}
 
void GameRenderer::showBuildSuccess(const Player& player, const LandPlot& landPlot) {
    cout << Formatter::buildSuccess(player, landPlot);
}
 
void GameRenderer::showBuildUpgradeSuccess(const Player& player, const LandPlot& landPlot) {
    cout << Formatter::buildUpgradeSuccess(player, landPlot);
}
 
void GameRenderer::showBuildNoEligible() {
    cout << Formatter::buildNoEligible();
}

// ── Auction (command 12 - LELANG) ─────────────────────────────────────
void GameRenderer::showAuctionStart(const PropertyPlot& property, const string& startingPlayer) {
    cout << Formatter::auctionStart(property, startingPlayer);
}

void GameRenderer::showAuctionTurn(string& username) {
    cout << Formatter::auctionTurn(username);
}

void GameRenderer::showAuctionHighestBid(int amount, const string& bidder) {
    cout << Formatter::auctionHighestBid(amount, bidder);
}

void GameRenderer::showAuctionResult(const PropertyPlot& property, string& winner, int cost) {
    cout << Formatter::auctionResult(property, winner, cost);
}

// ── Festival (command 13 - FESTIVAL) ──────────────────────────────────
void GameRenderer::showFestivalLanding(const Player& player) {
    cout << Formatter::festivalLanding(player);
}

void GameRenderer::showFestivalApply(int earlyRent, int currRent, int turn) {
    cout << Formatter::applyFestival(earlyRent, currRent, turn);
}

void GameRenderer::showFestivalEnhance(int earlyRent, int currRent, int turn) {
    cout << Formatter::enhanceFestival(earlyRent, currRent, turn);
}

void GameRenderer::showFestivalMaximum(int turn) {
    cout << Formatter::maximumFestival(turn);
}

void GameRenderer::showFestivalInvalidCode() {
    cout << Formatter::festivalInvalidCode();
}

void GameRenderer::showFestivalNotOwned() {
    cout << Formatter::festivalNotOwned();
}

// ── Bankruptcy & liquidation (command 14) ─────────────────────────────}
void GameRenderer::showDebtSummary(int playerMoney, int debt) {
    cout << Formatter::makeDebtSummary(playerMoney, debt);
}

void GameRenderer::showLiquidationPotential(string& status, const PropertyPlot& property) {
    cout << Formatter::potentialLiquidation(status, property);
}

void GameRenderer::showMaxPotential(int maxValue) {
    cout << Formatter::maxPotentialValue(maxValue);
}

void GameRenderer::showLiquidationStatus(int money, int debt) {
    cout << Formatter::makeLiquidationStatus(money, debt);
}

void GameRenderer::showSellPropertyList(int idx, const PropertyPlot& property) {
    cout << Formatter::sellPropertyList(idx, property);
}

void GameRenderer::showMortgagedList(int idx, const PropertyPlot& property) {
    cout << Formatter::mortgagedList(idx, property);
}

void GameRenderer::showSellPropertyToBank(const PropertyPlot& property, int money) {
    cout << Formatter::sellPropertyToBank(property, money);
}

void GameRenderer::showPayDebtSuccess(int debt, const Player& visitor, const Player& owner) {
    cout << Formatter::payDebtSuccess(debt, visitor, owner);
}

// ── Save / Load (command 15-16) ─────────────────────────────────────
void GameRenderer::successSaveFile(string& fileName) {
    cout << Formatter::successSaveFile(fileName);
}

void GameRenderer::failSaveFile(string& fileName) {
    cout << Formatter::failSaveFile(fileName);
}

void GameRenderer::successLoadFile(string& fileName, string& username) {
    cout << Formatter::successLoadFile(fileName, username);
}

void GameRenderer::fileNotFound(string& fileName) {
    cout << Formatter::fileNotFound(fileName);
}

void GameRenderer::failLoadFile() {
    cout << Formatter::failLoadFile();
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
void GameRenderer::showOnLandChanceCard(ChanceCard& card, string description) {
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

void GameRenderer::showActivateSkillCard(string name, string description) {
    cout << Formatter::activateSkillCard(name, description);
}

void GameRenderer::showEffectSkillCard(string label, string description) {
    cout << Formatter::effectSkillCard(label, description);
}

// ── Drop Kartu kemampuan (command 21 - DROP KARTU KEMAMPUAN) ────────────────────────────────────
void GameRenderer::showDropCardWarning(string& name) {
    cout << Formatter::dropCardWarning(name);
}

void GameRenderer::showDropCardAction(string& name) {
    cout << Formatter::dropCardAction(name);
}

// ── Game Exception ────────────────────────────────────
void GameRenderer::throwException(const GameException& ex) {
    cout << Formatter::throwException(ex);
}
