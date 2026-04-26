#include "views/Formatter.hpp"

 // ── Primitives ───────────────────────────────────────────────────────
string Formatter::moneyString(int value) {
    return "M" + to_string(value);
}

string Formatter::colorString(const Color& color) {
    if (color == Color::DEFAULT) {
        return "DEFAULT";
    } else if (color == Color::BROWN) {
        return "BROWN";
    } else if (color == Color::LIGHTBLUE) {
        return "LIGHT BLUE";
    } else if (color == Color::PINK) {
        return "PINK";
    } else if (color == Color::GRAY) {
        return "GRAY";
    } else if (color == Color::ORANGE) {
        return "ORANGE";
    } else if (color == Color::RED) {
        return "RED";
    } else if (color == Color::YELLOW) {
        return "YELLOW";
    } else if (color == Color::GREEN) {
        return "GREEN";
    } else if (color == Color::DARKBLUE) {
        return "DARK BLUE";
    } else {
        throw ColorSetNotOwnedException();
    }
}

string Formatter::statusString(const PropertyStatus& status) {
    if(status == PropertyStatus::BANK) {
        return "BANK";
    } else if (status == PropertyStatus::MORTGAGED) {
        return "MORTGAGED";
    } else if (status == PropertyStatus::OWNED) {
        return "OWNED";
    }
}

string Formatter::onLand(string name, string description) {
    return "Kamu mendarat di " + name + " " + description + '\n';
}

string Formatter::playerMoney(int money) {
    return "Uang kamu sekarang : " + moneyString(money);
}

// ── Input Handler ───────────────────────────────────────────────────────
string Formatter::promptInputMessage(string prompt){
    return prompt + " :";
}

string Formatter::promptYesNoMessage(string prompt) {
    return prompt + " (y/n) :";
}

 // ── Dice (command 2 & 3) ─────────────────────────────────────────────
string Formatter::showDiceRoll(const Player& player, const Dice& dice, string& position) {
    std::ostringstream oss;
    oss << "Mengocok dadu..." << endl;
    oss << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    oss << "Memajukan Bidak " << player.getUsername() << " sebanyak " << dice.getTotal() << "petak..." << endl;
    oss << onLand(position, "");

    return oss.str();
};

string Formatter::showControlDice(const Player& player, const Dice& dice, string& position) { 
    std::ostringstream oss;
    oss << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    oss << "Memajukan Bidak " << player.getUsername() << " sebanyak " << dice.getTotal() << "petak..." << endl;
    oss << onLand(position, "");

    return oss.str();
};

// ── Deed (command 4 - CETAK_AKTA) ────────────────────────────────────────────
string Formatter::makeDeedTable(const LandPlot& landPlot) {
    std::ostringstream oss;
    oss << "+===========================================================+" << endl;
    oss << "|                      AKTA KEPEMILIKAN                     |" << endl;
    oss << "| [" << colorString(landPlot.getColor()) << "] " << landPlot.getName() << " (" << landPlot.getCode() << ") " << " |" << endl;
    oss << "+===========================================================+" << endl;
    oss << "| Harga Beli        : " << moneyString(landPlot.getBuyPrice()) << " |" << endl;
    oss << "| Nilai Gadai       : " << moneyString(landPlot.getMortgageValue()) << " |" << endl;
    oss << "+-----------------------------------------------------------+" << endl;
    oss << "| Sewa (unimproved) : " << moneyString(landPlot.getRentPrice(0)) << " |" << endl;
    oss << "| Sewa (1 rumah)    : " << moneyString(landPlot.getRentPrice(1)) << " |" << endl;
    oss << "| Sewa (2 rumah)    : " << moneyString(landPlot.getRentPrice(2)) << " |" << endl;
    oss << "| Sewa (3 rumah)    : " << moneyString(landPlot.getRentPrice(3)) << " |" << endl;
    oss << "| Sewa (4 rumah)    : " << moneyString(landPlot.getRentPrice(4)) << " |" << endl;
    oss << "| Sewa (hotel)      : " << moneyString(landPlot.getRentPrice(5)) << " |" << endl;
    oss << "+-----------------------------------------------------------+" << endl;
    oss << "| Harga Rumah       : " << moneyString(landPlot.getUpgHousePrice()) << " |" << endl;
    oss << "| Harga Hotel       : " << moneyString(landPlot.getUpgHotelPrice()) << " |" << endl;
    oss << "+===========================================================+" << endl;
    
    PropertyStatus status = landPlot.getPropertyStatus();
    if(status == PropertyStatus::BANK) {
        oss << "| Status : " << statusString(status) << " |" << endl;
    } else {
        oss << "| Status : " << statusString(status) << " (" << landPlot.getOwner() << ") " << " |" << endl;
    }

    oss << "+===========================================================+" << endl;
    
    return oss.str();
}

string Formatter::deedNotFound(const string& code) {
    return "Petak \"" + code + "\" tidak ditemukan atau bukan properti.\n";
}

string Formatter::makePropertyList(const PropertyPlot& property) {
    return "- " + property.getName() + "(" + property.getCode() + ")" + "\n";
}

string Formatter::makePropertyList(const PropertyPlot& property, const Color& color) {
    return property.getName() + "(" + property.getCode() + ")" +  " [" + colorString(color) + "] ";
}

// ── Property list (command 5 - CETAK_PROPERTI) ───────────────────────
string Formatter::makePropertyList(PlotContext& ctx, const Player& player) {
    std::ostringstream oss;
    
    oss << "=== Properti Milik: " << player.getUsername() << " ===" << endl;

    const auto& ownedProperties = player.getOwnedProperties();

    if (ownedProperties.empty()) {
        oss << "Kamu belum memiliki properti apapun." << endl;
        return oss.str();
    }

    Color lastColor = Color::DEFAULT;
    int totalWealth = 0;

    for (const std::reference_wrapper<PropertyPlot>& plotRef : ownedProperties) {
        const PropertyPlot& plot = plotRef.get();
        const auto* land = dynamic_cast<const LandPlot*>(&plot);
        if (!land) continue;

        if (land->getColor() != lastColor) {
            lastColor = land->getColor();
            oss << "[" << colorString(lastColor) << "]" << endl;
        }

        int level = land->getLevel();
        int value = land->getBuyPrice();
        PropertyStatus stat = land->getPropertyStatus();
        totalWealth += value;

        oss << "  - ";
        oss << left << setw(35) << (land->getName() + " (" + land->getCode() + ")");

        if (level == 5) {
            oss << left << setw(10) << "Hotel";
        } else if (level > 0) {
            oss << left << setw(10) << (to_string(level) + " rumah");
        } else {
            oss << left << setw(10) << "";
        }

        oss << left << setw(8) << moneyString(value);

        if (stat == PropertyStatus::MORTGAGED) {
            oss << "MORTGAGED [M]";
        } else {
            oss << statusString(stat);
        }

        oss << endl;

        // Festival info per plot
        if (land->getFestivalDuration() > 0) {
            oss << "    [FESTIVAL AKTIF] Multiplier: x" << land->getFestivalMultiplier()
                << " | Sewa saat ini: " << moneyString(land->calculateRentPrice(ctx))
                << " | Sisa durasi: " << land->getFestivalDuration() << " giliran" << endl;
        }
    }

    oss << "Total kekayaan properti: " + moneyString(player.getTotalWealth()) << endl;

    return oss.str();
}

// ── Buy flow (command 6 - BELI) ───────────────────────────────────────────────
string Formatter::buyProperty(const Player& player, const LandPlot& landPlot) {
    std::ostringstream oss;

    oss << "Kamu mendarat di " << landPlot.getName() << " (" << landPlot.getCode() << ")!" << endl;
    oss << "+================================+" << endl;
    oss << "| [" << colorString(landPlot.getColor()) << "] "
        << landPlot.getName() << " (" << landPlot.getCode() << ")" << " |" << endl;
    oss << "+================================+" << endl;
    oss << "| Harga Beli    : " << moneyString(landPlot.getBuyPrice()) << " |" << endl;
    oss << "+--------------------------------+" << endl;
    oss << "| Sewa dasar    : " << moneyString(landPlot.getRentPrice(0)) << " |" << endl;
    oss << "| Sewa 1 rumah  : " << moneyString(landPlot.getRentPrice(1)) << " |" << endl;
    oss << "| Sewa 2 rumah  : " << moneyString(landPlot.getRentPrice(2)) << " |" << endl;
    oss << "| Sewa 3 rumah  : " << moneyString(landPlot.getRentPrice(3)) << " |" << endl;
    oss << "| Sewa 4 rumah  : " << moneyString(landPlot.getRentPrice(4)) << " |" << endl;
    oss << "| Sewa hotel    : " << moneyString(landPlot.getRentPrice(5)) << " |" << endl;
    oss << "+================================+" << endl;
    oss << "Uang kamu saat ini: " << moneyString(player.getCash()) << endl;
    oss << "Apakah kamu ingin membeli properti ini seharga "
        << moneyString(landPlot.getBuyPrice()) << "? (y/n): ";

    return oss.str();
}

string Formatter::buySuccess(const Player& player, const LandPlot& landPlot) {
    std::ostringstream oss;
    oss << landPlot.getName() << " kini menjadi milikmu!" << endl;
    oss << "Uang tersisa: " << moneyString(player.getCash() - landPlot.getBuyPrice()) << endl;
    return oss.str();
}

string Formatter::buyFailed() {
    std::ostringstream oss;
    oss << "Properti ini akan masuk ke sistem lelang..." << endl;
    return oss.str();
}

string Formatter::buyStation(const StationPlot& station) {
    std::ostringstream oss;
    oss << onLand(station.getName(), " (" + station.getCode() + ")!");
    oss << "Belum ada yang menginjaknya duluan, stasiun ini kini menjadi milikmu!" << endl;
    return oss.str();
}

string Formatter::buyUtility(const UtilityPlot& utility) {
    std::ostringstream oss;
    oss << onLand(utility.getName(), "!");
    oss << "Belum ada yang menginjaknya duluan, " << utility.getName()
        << " kini menjadi milikmu!" << endl;
    return oss.str();
}

// ── Pay rent (command 7 - BAYAR_SEWA) ────────────────────────────────────────
string Formatter::makePayRent(PlotContext& ctx, const PropertyPlot& property) { // TODO: calculaterentPrice() harus const
    std::ostringstream oss;
    string description = " (" + property.getCode() + ")," + " milik " + property.getOwner()->getUsername();
    oss << onLand(property.getName(), description + "!");
    if (property.getLevel() == 5) {
        oss << "Kondisi : 1 Hotel" << endl; 
    } else if (property.getLevel() > 0)  {
        oss << "Kondisi : " << property.getLevel() << "Rumah" << endl;
    } 

    int visitorMoney = ctx.getCurrentPlayer().getTotalWealth();
    int ownerMoney = property.getOwner()->getTotalWealth();
    oss << "Sewa      : " << moneyString(property.calculateRentPrice(ctx)) << endl << endl;
    oss << "Uang Kamu : " << moneyString(ctx.getCurrentPlayer().getCash()) << " -> " << ctx.getCurrentPlayer().getCash() - property.calculateRentPrice(ctx) << endl;
    oss << "Uang Pemain " << property.getOwner()->getUsername() << " : " <<  property.getOwner()->getCash() + property.calculateRentPrice(ctx) << endl;

    return oss.str();
}

string Formatter::mortgagedPlot(const PropertyPlot& property) {
    std::ostringstream oss;
    string description = " (" + property.getCode() + ")," + " milik " + property.getOwner()->getUsername();
    oss << onLand(property.getName(), description + "!"); 
    oss << "Properti ini sedang digadaikan [M]. Tidak ada sewa yang dikenakan." << endl; 

    return oss.str();
}

string Formatter::cannotPayRent(int rentAmount, int playerCash) {
    std::ostringstream oss;
    oss << "Kamu tidak mampu membayar sewa penuh! (" << moneyString(rentAmount) << ")" << endl;
    oss << "Uang kamu saat ini: " << moneyString(playerCash) << endl;
    return oss.str();
}

// ── Pay tax (command 8 - BAYAR_PAJAK) ────────────────────────────────────────
string Formatter::payIncomeTax(int flat, int percentage) { 
    std::ostringstream oss;
    oss << onLand("Pajak Penghasilan", "(PPH) !");
    oss << "Pilih opsi pembayaran pajak :" << endl;
    oss << "1. Bayar flat " << moneyString(flat) << endl;
    oss << "2. Bayar " << percentage << "% " << "dari total kekayaan" << endl;
    oss << "(Pilih sebelum menghitung kekayaan!)" << endl;

    return oss.str();
}

string Formatter::payIncomeTaxResult(int totalWealth, int cashBefore, int percentage) {
    int taxAmount = totalWealth * percentage / 100;
    int cashAfter  = cashBefore - taxAmount;
    std::ostringstream oss;
    oss << "Total kekayaan kamu:" << endl;
    oss << "- Uang tunai          : " << moneyString(cashBefore) << endl;
    oss << "- Harga beli properti : (sudah dihitung)" << endl;
    oss << "Total     : " << moneyString(totalWealth) << endl;
    oss << "Pajak " << percentage << "% : " << moneyString(taxAmount) << endl;
    oss << "Uang kamu : " << moneyString(cashBefore) << " -> " << moneyString(cashAfter) << endl;
    return oss.str();
}

string Formatter::payFlatTax(int flat, int money) {
    std::ostringstream oss;
    oss << "Kamu membayar bayar pajak flat " << flat << "!" << endl;
    oss << playerMoney(money) << endl;

    return oss.str();
}

string Formatter::failPayFlatTax(int flat, int money) { 
    std::ostringstream oss;
    oss << "Kamu tidak mampu bayar pajak flat " << flat << "!" << endl;
    oss << playerMoney(money) << endl;

    return oss.str();
}

string Formatter::payLuxuryTax(int startMoney, int finalMoney) { 
    std::ostringstream oss;
    oss << onLand("Pajak Barang Mewah", "(PBM) !");
    oss << "Pajak sebesar M150 langsung dipotong" << endl;
    oss << "Uang kamu: " << moneyString(startMoney) << " -> " << moneyString(finalMoney) << endl;

    return oss.str();    
}

string Formatter::failPayLuxuryTax(int money) { 
    std::ostringstream oss;
    oss << onLand("Pajak Barang Mewah", "(PBM) !");
    oss << "Pajak sebesar M150 langsung dipotong" << endl;
    oss << "Kamu tidak mampu membayar pajak" << endl;
    oss << "Uang kamu: " << moneyString(money) << endl;

    return oss.str();   
}

// ── Mortgage (command 9 - GADAI [TODO REVISION]) ──────────────────────────────────────
string Formatter::makeCanMortgagedList(const PropertyPlot& property, int money) {
    std::ostringstream oss;
    oss << makePropertyList(property, property.getColor()) << "Nilai Gadai : " << moneyString(money) << endl;

    return oss.str();
}

/**
 * TODO: Revisi nanti
 * 
 */
string Formatter::successMortgage(string name, int moneyReceived, int moneyTotal) {
    std::ostringstream oss;
    oss << name << "berhasil digadaikan." << endl;
    oss << "Kamu menerima " << moneyString(moneyReceived) << " dari Bank" << endl;
    oss << playerMoney(moneyTotal) << endl;
    oss << "Catatan : Sewa tidak dapat dipungut dari properti yang digadaikan" << endl;

    return oss.str();
}

string Formatter::failedMortgage(const LandPlot& landplot) {
    std::ostringstream oss;
    oss << landplot.getName() << " tidak dapat digadaikan!" << endl;
    oss << "Masih terdapat bangunan di color group [" << colorString(landplot.getColor()) << "]." << endl;
    oss << "Bangunan harus dijual terlebih dahulu." << endl;
    oss << endl;
    oss << "Daftar bangunan di color group [" << colorString(landplot.getColor()) << "] :" << endl;
    
    return oss.str();
}

string Formatter::makeSellBuildingOption(int idx, const LandPlot& landPlot) {
    std::ostringstream oss;
    oss << idx << ". " << landPlot.getName() << " (" << landPlot.getCode() << ") " << "- " << landPlot.getLevel() << " " << landPlot.getBuildingType() << " -> " << "Nilai jual bangunan : " << moneyString(landPlot.getSellBuildingPrice()) << endl;

    return oss.str();
}

string Formatter::sellProperty(string& name, int cost) {
    return "Bangunan " + name + " terjual. " + "Kamu menerima " + moneyString(cost) + "\n";
}

// ── Redeem (command 10 - TEBUS) ───────────────────────────────────────────────
string Formatter::makeRedeemList(const PropertyPlot& property) { 
    std::ostringstream oss;
    oss << makePropertyList(property, property.getColor()) << "[M] " << "Harga Tebus : " << moneyString(property.getMortgageValue()) << endl;
    
    return oss.str();
}

string Formatter::redeemNoEligible() {
    return "Tidak ada properti yang sedang digadaikan.\n";
}

string Formatter::successRedeemProperty(const PropertyPlot& property, int money) { 
    std::ostringstream oss;
    oss << property.getName() << " berhasil ditebus!" << endl;
    oss << "Kamu membayar " << moneyString(property.getMortgageValue()) << " ke Bank" << endl;
    oss << playerMoney(money) << endl;

    return oss.str();
}

string Formatter::failedRedeemProperty(const PropertyPlot& property, int money) { 
    std::ostringstream oss;
    oss << "Uang kamu tidak cukup untuk menebus " << property.getName() << endl;
    oss << "Harga tebus : " << moneyString(property.getMortgageValue()) << " | " << "Uang kamu : " << moneyString(money) << endl;

    return oss.str();
}

// ── Build flow (command 11 - BANGUN) ─────────────────────────────────
string Formatter::buildGroupList(const Player& player) {
    std::ostringstream oss;
    
    oss << "=== Color Group yang Memenuhi Syarat ===" << endl;

    const auto& ownedProperties = player.getOwnedProperties();

    // Group by color
    map<Color, vector<const LandPlot*>> colorGroups;
    for (const auto& plotRef : ownedProperties) {
        const Plot& plot = plotRef.get();
        const auto* land = dynamic_cast<const LandPlot*>(&plot);
        if (!land) continue;
        colorGroups[land->getColor()].push_back(land);
    }

    // Filter only eligible groups (all plots in group owned by player)
    map<Color, vector<const LandPlot*>> eligibleGroups;
    for (const auto& entry : colorGroups) {
        const auto& plots = entry.second;
        bool allOwned = all_of(plots.begin(), plots.end(),
            [](const LandPlot* p) { return p->getPropertyStatus() == PropertyStatus::OWNED; });
        if (allOwned) eligibleGroups[entry.first] = plots;
    }

    if (eligibleGroups.empty()) {
        oss << buildNoEligible();
        return oss.str();
    }

    int idx = 1;
    for (const auto& entry : eligibleGroups) {
        const Color& color = entry.first;
        const auto& plots = entry.second;
        oss << idx++ << ". [" << colorString(color) << "]" << endl;
        for (const LandPlot* land : plots) {
            int level = land->getLevel();
            string levelStr = (level == 5) ? "Hotel" : (to_string(level) + " rumah");
            oss << "   - ";
            oss << left << setw(30) << (land->getName() + " (" + land->getCode() + ")");
            oss << ": " << levelStr;
            oss << " (Harga rumah: " << moneyString(land->getUpgHousePrice()) << ")" << endl;
        }
    }

    oss << "Uang kamu saat ini : " << moneyString(player.getCash()) << endl;
    oss << "Pilih nomor color group (0 untuk batal): ";
    return oss.str();
}

string Formatter::buildPlotList(const Player& player, const Color& color) {
    std::ostringstream oss;

    const auto& ownedProperties = player.getOwnedProperties();

    vector<const LandPlot*> plots;
    for (const auto& plotRef : ownedProperties) {
        const Plot& plot = plotRef.get();
        const auto* land = dynamic_cast<const LandPlot*>(&plot);
        if (land && land->getColor() == color) plots.push_back(land);
    }

    bool allFourHouses = true;
    for (const LandPlot* land : plots) {
        if (land->getLevel() != 4) {
            allFourHouses = false;
            break;
        }
    }

    oss << "Color group [" << colorString(color) << "]:" << endl;

    for (const LandPlot* land : plots) {
        int level = land->getLevel();
        string levelStr = (level == 5) ? "Hotel" : (to_string(level) + " rumah");

        oss << "- ";
        oss << left << setw(35) << (land->getName() + " (" + land->getCode() + ")");
        oss << ": " << levelStr;

        if (level == 5) {
            oss << "  <- sudah maksimal, tidak dapat dibangun";
        } else if (level == 4) {
            oss << "  <- siap upgrade ke hotel";
        } else {
            oss << "  <- dapat dibangun";
        }

        oss << endl;
    }

    if (allFourHouses) {
        oss << "Seluruh color group [" << colorString(color) << "] sudah memiliki 4 rumah. Siap di-upgrade ke hotel!" << endl;
    }

    oss << "Pilih petak (0 untuk batal): ";
    return oss.str();
}

string Formatter::buildSuccess(const Player& player, const LandPlot& landPlot) {
    std::ostringstream oss;
    oss << "Kamu membangun 1 rumah di " << landPlot.getName()
        << ". Biaya: " << moneyString(landPlot.getUpgHousePrice()) << endl;
    oss << "Uang tersisa: " << moneyString(player.getCash() - landPlot.getUpgHousePrice()) << endl;
    return oss.str();
}

string Formatter::buildUpgradePrompt(const LandPlot& landPlot) {
    std::ostringstream oss;
    oss << "Upgrade ke hotel? Biaya: " << moneyString(landPlot.getUpgHotelPrice()) << " (y/n): ";
    return oss.str();
}

string Formatter::buildUpgradeSuccess(const Player& player, const LandPlot& landPlot) {
    std::ostringstream oss;
    oss << landPlot.getName() << " di-upgrade ke Hotel!" << endl;
    oss << "Uang tersisa: " << moneyString(player.getCash() - landPlot.getUpgHotelPrice()) << endl;
    return oss.str();
}

string Formatter::buildNoEligible() {
    std::ostringstream oss;
    oss << "Tidak ada color group yang memenuhi syarat untuk dibangun." << endl;
    oss << "Kamu harus memiliki seluruh petak dalam satu color group terlebih dahulu." << endl;
    return oss.str();
}

// ── Auction (command 12 - LELANG) ─────────────────────────────────────────────
string Formatter::auctionStart(const PropertyPlot& property, const string& startingPlayer) {
    std::ostringstream oss;
    oss << "Properti " << property.getName() << " (" << property.getCode() << ") akan dilelang!" << endl;
    oss << "Urutan lelang dimulai dari pemain setelah " << startingPlayer << "." << endl;
    return oss.str();
}

string Formatter::auctionTurn(string& username) {
    std::ostringstream oss;
    oss << "Giliran : " << username << endl;
    oss << "Aksi (PASS / BID <jumlah>)" << endl;

    return oss.str();
};

string Formatter::auctionHighestBid(int amount, const string& bidder) {
    return "Penawaran tertinggi: " + moneyString(amount) + " (" + bidder + ")\n";
}

string Formatter::auctionResult(const PropertyPlot& property, string& username, int cost) { 
    std::ostringstream oss;
    oss << "Lelang selesai!" << endl;
    oss << "Pemenang    : " << username << endl;
    oss << "Harga akhir : " << moneyString(cost) << endl;
    oss << endl;
    oss << "Properti " << property.getName() << " (" << property.getCode() << ") " << "dimiliki " << username << endl;

    return oss.str();    
}

// ── Festival (command 13 - FESTIVAL) ──────────────────────────────────────────
string Formatter::festivalLanding(const Player& player) {
    std::ostringstream oss;
    oss << "Kamu mendarat di petak Festival!" << endl;
    oss << "Daftar properti milikmu:" << endl;
    for (const auto& ref : player.getOwnedProperties()) {
        const PropertyPlot& prop = ref.get();
        oss << "- " << prop.getName() << " (" << prop.getCode() << ")" << endl;
    }

    return oss.str();
}

string Formatter::applyFestival(int earlyRent, int currRent, int turn) {
    std::ostringstream oss;
    oss << "Efek Festival aktif !" << endl;
    oss << "Sewa awal      : " << moneyString(earlyRent) << endl;
    oss << "Sewa sekarang  : " << moneyString(currRent) << endl;
    oss << "Durasi         : " << turn << " giliran" << endl;

    return oss.str();
}

string Formatter::enhanceFestival(int earlyRent, int currRent, int turn) {
    std::ostringstream oss;
    oss << "Efek Festival Diperkuat !" << endl;
    oss << "Sewa awal               : " << moneyString(earlyRent) << endl;
    oss << "Sewa sekarang           : " << moneyString(currRent) << endl;
    oss << "Durasi di-reset menjadi : " << turn << " giliran" << endl;

    return oss.str();    
}

string Formatter::maximumFestival(int turn) {
    std::ostringstream oss;
    oss << "Efek sudah maksimum (harga sewa sudah digandakan tiga kali)" << endl;
    oss << "Durasi di-reset menjadi : " << turn << " giliran" << endl;

    return oss.str();   
}

string Formatter::festivalInvalidCode() {
    return "Kode properti tidak valid!\n";
}

string Formatter::festivalNotOwned() {
    return "Properti bukan milikmu!\n";
}


// ── Bankruptcy & liquidation (command 14 [TODO REVISION]) ─────────────────────────────
string Formatter::makeDebtSummary(int playerMoney, int debt) {
    std::ostringstream oss;
    oss << "Uang kamu       : " << moneyString(playerMoney) << endl;
    oss << "Totak kewajiban : " << moneyString(playerMoney + debt) << endl;
    oss << "Kekurangan      : " << moneyString(debt) << endl;

    return oss.str();
}

string Formatter::potentialLiquidation(string& status, const PropertyPlot& property) {
    std::ostringstream oss;
    oss << status << " " << makePropertyList(property, property.getColor()) << "-> " << moneyString(property.calculateTotalValue()) << endl;

    return oss.str();
}

string Formatter::maxPotentialValue(int maxValue) {
    return "Total potensi -> " + to_string(maxValue) + '\n';
}

string Formatter::makeLiquidationStatus(int money, int debt) {
    return playerMoney(money) + " | " + "Kewajiban : " + moneyString(debt) + "\n";
}

string Formatter::sellPropertyList(int idx, const PropertyPlot& property) {
    std::ostringstream oss;
    oss << idx << ". " << makePropertyList(property, property.getColor()) << "Harga Jual : " << property.calculateTotalValue() << endl;

    return oss.str();  
}

string Formatter::mortgagedList(int idx, const PropertyPlot& property) {
    std::ostringstream oss;
    oss << idx << ". " << makePropertyList(property, property.getColor()) << "Harga Gadai : " << property.getMortgageValue() << endl;

    return oss.str(); 
}

string Formatter::sellPropertyToBank(const PropertyPlot& property, int money) {
    std::ostringstream oss;
    oss << property.getName() << " terjual ke Bank. Kamu menerima " << moneyString(property.calculateTotalValue()) << endl;
    oss << playerMoney(money + property.calculateTotalValue()) << endl;

    return oss.str();
}

string Formatter::payDebtSuccess(int debt, const Player& visitor, const Player& owner) {
    std::ostringstream oss;
    oss << "Kewajiban " << moneyString(debt) << " terpenuhi. Membayar ke " << owner.getUsername() << "..." << endl;
    oss << playerMoney(visitor.getCash()) << " -> " << playerMoney(visitor.getCash() - debt) << endl;
    oss << "Uang " << owner.getUsername() << " : " << playerMoney(owner.getCash()) << " -> " << playerMoney(owner.getCash() - debt) << endl;

    return oss.str();
};

// ── Save / Load (command 15-16) ───────────────────────────────────────
string Formatter::successLoadFile(string& fileName, string& username) {
    std::ostringstream oss;
    oss << "Memuat permainan..." << endl;
    oss << "Permainan berhasil dimuat. Melanjutkan giliran " << username << endl;

    return oss.str();
}

string Formatter::fileNotFound(string& fileName) {
    std::ostringstream oss;
    oss << "File " << fileName << " tidak ditemukan." << endl;

    return oss.str();
}

string Formatter::failLoadFile() {
    std::ostringstream oss;
    oss <<  "Gagal memuat file! File rusak atau format tidak dikenali." << endl;

    return oss.str();
}

string Formatter::successSaveFile(string& fileName) {
    std::ostringstream oss;
    oss << "Menyimpan permainan..." << endl;
    oss << "Permainan berhasil disimpan ke: " << fileName << endl;

    return oss.str();
}

string Formatter::failSaveFile(string& fileName) {
    return "Gagal menyimpan file! Pastikan direktori dapat ditulis.\n";
}

// ── Log (command 17 - CETAK_LOG) ─────────────────────────────────────
string Formatter::showLogger(const LogEntry& log) {
    return "[" + to_string(log.turn) + "] " + log.username + " | " + log.actionType + " | " + log.detail + "\n";
}

// ── Win (command 18 - MENANG) ─────────────────────────────────────────
string Formatter::makePlayerList(const Player& player) { 
    std::ostringstream oss;
    oss << "Player " << player.getUsername() << " ----------" << endl;
    oss << "Money    : " << player.getTotalWealth() << endl;
    oss << "Property : " << player.getOwnedProperties().size() << endl;
    oss << "Cards    : " << player.getOwnedCards().size() << endl;
    oss << endl;

    return oss.str();
}

string Formatter::winner(string username) {
    return "Pemenang : " + username + "\n";
}

string Formatter::finalPlayer(string username) {
    std::ostringstream oss;
    oss << "Pemain tersisa : " << endl;
    oss << "- " << username << endl;

    return oss.str();    
}

// ── Chance / Community Chest (command 19) ─────────────────────────────
string Formatter::chanceCardPlot(ChanceCard& card, string& description) {
    std::ostringstream oss;
    oss << onLand("Petak Kesempatan", "!");
    oss << "Mengambil kartu..." << endl;
    oss << "Kartu : " << card.getName() << endl;
    oss << "\"" << card.getDescription() << "\"" << endl;
    oss << description << endl;

    return oss.str();
}

string Formatter::communityChestPlot(CommunityChestCard& card, int cost, int currMoney) {
    std::ostringstream oss;
    oss << onLand("Petak Dana Umum", "!");
    oss << "Mengambil kartu..." << endl;
    oss << "Kartu : " << card.getName() << endl;
    oss << "\"" << card.getDescription() << "\"" << endl;

    if(currMoney >= cost) {
        oss << "Kamu membayar " << moneyString(cost) << " ke Bank. Sisa Uang = " << moneyString(currMoney) << endl;
    } else {
        oss << "Kamu tidak mampu bayar (" << moneyString(cost) << ") " << endl;
        oss << playerMoney(currMoney) << endl;
    }

    return oss.str();
}

// ── Skill cards (command 20 - GUNAKAN_KEMAMPUAN) ─────────────────────
string Formatter::makeCardList(int idx, const string& name, const string& description) { 
    return to_string(idx + 1) + ". " + name + "---" + description + "\n";
}

string Formatter::usedSkillCard(bool isUsed) {
    if(isUsed) {
        return "Kamu sudah menggunakan kartu kemampuan pada giliran ini!\nPenggunaan kartu dibatasi maksimal 1 kali dalam 1 giliran.\n";   
    }

    return "Kartu kemampuan hanya bisa digunakan SEBELUM melempar dadu.\n";
}

string Formatter::activateSkillCard(string& name, string& description) {
    return name + " diaktifkan! " + description + "\n";
}

string Formatter::effectSkillCard(string& label, string& description) {
    return "[" + label + "] " + description;
}

// ── Drop card (command 21 - DROP KARTU) ──────────────────────────────
string Formatter::dropCardWarning(const string& name) {
    std::ostringstream oss;
    oss << "Kamu mendapatkan 1 kartu acak baru !" << endl;
    oss << "Kartu yang didapat : " << name << endl;
    oss << "PERINGATAN: Kamu sudah memiliki 3 kartu di tangan (Maksimal 3)!" << endl;
    oss << "Kamu diwajibkan membuang 1 kartu." << endl;

    return oss.str();
}

string Formatter::dropCardAction(const string& name) {
    return name + "telah dibuang. Sekarang kamu memiliki 3 kartu di tangan.\n";
}

// ── Special plots ─────────────────────────────────────────────────
string Formatter::landOnGo() {
    std::ostringstream oss;
    oss << onLand("Petak Mulai (GO)", "!");
    return oss.str();
}
 
string Formatter::landOnFreePark() {
    std::ostringstream oss;
    oss << onLand("Petak Bebas Parkir", "!");
    return oss.str();
}
 
string Formatter::landOnGoPrison() {
    std::ostringstream oss;
    oss << onLand("Petak Pergi ke Penjara", "!");
    oss << "Kamu masuk penjara." << endl;
    return oss.str();
}
 
string Formatter::landOnPrisonVisit() {
    std::ostringstream oss;
    oss << onLand("Petak Penjara", " - Hanya Berkunjung");
    return oss.str();
}
 
string Formatter::landOnPrisonJailed(int jailTurnsLeft) {
    std::ostringstream oss;
    oss << "Kamu sedang di dalam Penjara." << endl;
    oss << "Sisa giliran percobaan: " << jailTurnsLeft << endl;
    oss << "Pilihan untuk keluar:" << endl;
    oss << "  1. Bayar denda ke Bank sebelum melempar dadu." << endl;
    oss << "  2. Gunakan kartu 'Bebas dari Penjara' (jika punya)." << endl;
    oss << "  3. Lempar dadu dan harapkan angka double." << endl;
    if (jailTurnsLeft <= 0) {
        oss << "Batas percobaan habis! Kamu WAJIB membayar denda." << endl;
    }
    return oss.str();
}

// ── Exception ──────────────────────────────────────────────────────────────
std::string Formatter::throwException(const GameException& e) {
    return e.what();
}