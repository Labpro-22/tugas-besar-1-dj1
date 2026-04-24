#include "core/Formatter.hpp"

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
    return "Uang kamu sekarang : " + moneyString(money) + '\n';
}

string Formatter::showDiceRoll(string username, const Dice& dice, string position) {
    std::ostringstream oss;
    oss << "Mengocok dadu..." << endl;
    oss << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    oss << "Memajukan Bidak " << username << " sebanyak " << dice.getTotal() << "petak..." << endl;
    oss << onLand(position, "");

    return oss.str();
};

string Formatter::showControlDice(string username, const Dice& dice, string position) {
    std::ostringstream oss;
    oss << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    oss << "Memajukan Bidak " << username << " sebanyak " << dice.getTotal() << "petak..." << endl;
    oss << onLand(position, "");

    return oss.str();
};

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
        // TODO: getOwner() ??
        //oss << "| Status : " << statusString(status) << " (" << landPlot.getOwner() << ") " << " |" << endl;
    }

    oss << "+===========================================================+" << endl;
    
    return oss.str();
}

string Formatter::makePropertyList(string name, string code) {
    return "- " + name + "(" + code + ")" + "\n";
}

string Formatter::makePropertyList(const Player& player) {
    std::ostringstream oss;
    
    oss << "=== Properti Milik: " << player.getUsername() << " ===" << endl;

    const auto& ownedProperties = player.getOwnedProperties();

    if (ownedProperties.empty()) {
        oss << "Kamu belum memiliki properti apapun." << endl;
        return oss.str();
    }

    Color lastColor = Color::DEFAULT;
    int totalWealth = 0;

    for (const std::reference_wrapper<Plot>& plotRef : ownedProperties) {
        const Plot& plot = plotRef.get();
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
                << " | Sewa saat ini: " << moneyString(land->calculateRentPrice())
                << " | Sisa durasi: " << land->getFestivalDuration() << " giliran" << endl;
        }
    }

    oss << "Total kekayaan properti: " + moneyString(player.getTotalWealth()) << endl;

    return oss.str();
}

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
    oss << "Kamu mendarat di " << station.getName() << " (" << station.getCode() << ")!" << endl;
    oss << "Belum ada yang menginjaknya duluan, stasiun ini kini menjadi milikmu!" << endl;
    return oss.str();
}

string Formatter::buyUtility(const UtilityPlot& utility) {
    std::ostringstream oss;
    oss << "Kamu mendarat di " << utility.getName() << "!" << endl;
    oss << "Belum ada yang menginjaknya duluan, " << utility.getName()
        << " kini menjadi milikmu!" << endl;
    return oss.str();
}

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
    for (const auto& [color, plots] : colorGroups) {
        bool allOwned = all_of(plots.begin(), plots.end(),
            [](const LandPlot* p) { return p->getPropertyStatus() == PropertyStatus::OWNED; });
        if (allOwned) eligibleGroups[color] = plots;
    }

    if (eligibleGroups.empty()) {
        oss << buildNoEligible();
        return oss.str();
    }

    int idx = 1;
    for (const auto& [color, plots] : eligibleGroups) {
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

string Formatter::buildUpgradePrompt(const Player& player, const LandPlot& landPlot) {
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

string Formatter::makeMortgagedList(string name, string code, Color& color, int money) {
    std::ostringstream oss;
    oss << name << " (" << code << ") " << "[" << colorString(color) << "] " << "[M] " << "Harga Tebus : " << moneyString(money) << endl;
    
    return oss.str();
}

string Formatter::successRedeemProperty(string name, int cost, int money) {
    std::ostringstream oss;
    oss << name << " berhasil ditebus!" << endl;
    oss << "Kamu membayar " << moneyString(cost) << " ke Bank" << endl;
    oss << playerMoney(money);

    return oss.str();
}

string Formatter::failedRedeemProperty(string name, int cost, int money) {
    std::ostringstream oss;
    oss << "Uang kamu tidak cukup untuk menebus " << name << endl;
    oss << "Harga tebus : " << moneyString(cost) << " | " << "Uang kamu : " << moneyString(money) << endl;

    return oss.str();
}

string Formatter::auctionTurn(string username) {
    std::ostringstream oss;
    oss << "Giliran : " << username << endl;
    oss << "Aksi (PASS / BID <jumlah>)" << endl;

    return oss.str();
};

string Formatter::auctionResult(string username, int cost, string plotName, string code) {
    std::ostringstream oss;
    oss << "Lelang selesai!" << endl;
    oss << "Pemenang    : " << username << endl;
    oss << "Harga akhir : " << moneyString(cost) << endl;
    oss << endl;
    oss << "Properti " << plotName << " (" << code << ") " << "dimiliki " << username << endl;

    return oss.str();    
}

string Formatter::makeCanMortgagedList(string name, string code, Color& color, int money) {
    std::ostringstream oss;
    oss << name << " (" << code << ") " << "[" << colorString(color) << "] " << "Nilai Gadai : " << moneyString(money) << endl;

    return oss.str();
}

string Formatter::successMortgage(string name, int moneyRecieved, int moneyTotal) {
    std::ostringstream oss;
    oss << name << "berhasil digadaikan." << endl;
    oss << "Kamu menerima " << moneyString(moneyRecieved) << " dari Bank" << endl;
    oss << playerMoney(moneyTotal);
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
}

string Formatter::makeSellBuildingOption(int idx, string name, string code, int buildCount, string buildType, int buildValue) {
    std::ostringstream oss;
    oss << idx << ". " << name << " (" << code << ") " << "- " << buildCount << " " << buildType << " -> " << "Nilai jual bangunan : " << moneyString(buildValue) << endl;

    return oss.str();
}

string Formatter::sellProperty(string name, int cost) {
    return "Bangunan " + name + " terjual. " + "Kamu menerima " + moneyString(cost) + "\n";
}

string Formatter::makePayRent(const Player& visitor, const Player& owner, int level, int rentValue, string code) {
    std::ostringstream oss;
    oss << "Kamu mendarat di " << code << ", milik " << owner.getUsername() << "!" << endl;
    if (level == 5) {
        oss << "Kondisi : 1 Hotel" << endl; 
    } else if (level > 0)  {
        oss << "Kondisi : " << level << "Rumah" << endl;
    } 

    int visitorMoney = visitor.getTotalWealth();
    int ownerMoney = owner.getTotalWealth();
    oss << "Sewa      : " << moneyString(rentValue) << endl << endl;
    oss << "Uang Kamu : " << moneyString(visitorMoney) << " -> " << visitorMoney - rentValue << endl;
    oss << "Uang Pemain " << owner.getUsername() << " : " <<  ownerMoney + rentValue << endl;

    return oss.str();
}

string Formatter::mortgagedPlot(string ownerName, string plotName, string code) {
    std::ostringstream oss;
    oss << "Kamu mendarat di " << plotName << " (" << code << ")," << " milik " << ownerName << endl;
    oss << "Properti ini sedang digadaikan [M]. Tidak ada sewa yang dikenakan." << endl; 

    return oss.str();
}

string Formatter::payIncomeTax() {
    std::ostringstream oss;
    oss << "Kamu mendarat di Pajak Penghasilan (PPH) !" << endl;
    oss << "Pilih opsi pembayaran pajak :" << endl;
    oss << "1. Bayar flat M150" << endl;
    oss << "2. Bayar 10% dari total kekayaan" << endl;
    oss << "(Pilih sebelum menghitung kekayaan!)" << endl;

    return oss.str();
}

string Formatter::payLuxuryTax(int startMoney, int finalMoney) {
    std::ostringstream oss;
    oss << "Kamu mendarat di Pajak Barang Mewah (PBM) !" << endl;
    oss << "Pajak sebesar M150 langsung dipotong" << endl;
    oss << "Uang kamu: " << moneyString(startMoney) << " -> " << moneyString(finalMoney) << endl;

    return oss.str();    
}

string Formatter::failPayTax(int money) {
    std::ostringstream oss;
    oss << "Kamu tidak mampu bayar pajak flat M150 !" << endl;
    oss << playerMoney(money);

    return oss.str();
}

string Formatter::applyFestival(int startMoney, int finalMoney, int turn) {
    std::ostringstream oss;
    oss << "Efek Festival aktif !" << endl;
    oss << "Sewa awal      : " << moneyString(startMoney) << endl;
    oss << "Sewa sekarang  : " << moneyString(finalMoney) << endl;
    oss << "Durasi         : " << turn << " giliran" << endl;

    return oss.str();
}

string Formatter::enhanceFestival(int startMoney, int finalMoney, int turn) {
    std::ostringstream oss;
    oss << "Efek Festival Diperkuat !" << endl;
    oss << "Sewa awal               : " << moneyString(startMoney) << endl;
    oss << "Sewa sekarang           : " << moneyString(finalMoney) << endl;
    oss << "Durasi di-reset menjadi : " << turn << " giliran" << endl;

    return oss.str();    
}

string Formatter::maximumFestival(int turn) {
    std::ostringstream oss;
    oss << "Efek sudah maksimum (harga sewa sudah digandakan tiga kali)" << endl;
    oss << "Durasi di-reset menjadi : " << turn << " giliran" << endl;

    return oss.str();   
}

string Formatter::winner(string username) {
    return "Pemenang : " + username + "\n";
}

string Formatter::makePlayerList(string name, int money, int totalProperty, int totalCard) {
    std::ostringstream oss;
    oss << "Player " << name << " ----------" << endl;
    oss << "Money    : " << money << endl;
    oss << "Property : " << totalProperty << endl;
    oss << "Cards    : " << totalCard << endl;
    oss << endl;

    return oss.str();
}

string Formatter::finalPlayer(string username) {
    std::ostringstream oss;
    oss << "Pemain tersisa : " << endl;
    oss << "- " << username << endl;

    return oss.str();    
}

string Formatter::communityChestPlot(const CommunityChestCard& card, int cost, int currMoney) {
    std::ostringstream oss;
    oss << "Mengambil kartu..." << endl;
    oss << "Kartu : " << card.getName() << endl;
    oss << "\"" << card.getDescription() << "\"" << endl;

    if(currMoney >= cost) {
        oss << "Kamu membayar " << moneyString(cost) << " ke Bank. Sisa Uang = " << moneyString(currMoney - cost) << endl;
    } else {
        oss << "Kamu tidak mampu bayar (" << moneyString(cost) << ") " << endl;
        oss << playerMoney(currMoney);
    }

    return oss.str();
}

string showLogger(int turn, string username, string actionType, string detail) {
    return "[" + to_string(turn) + "] " + username + " | " + actionType + " | " + detail + "\n";
};

string Formatter::makeCardList(int idx, string name, string description) {
    return to_string(idx + 1) + ". " + name + "---" + description + "\n";
}

string Formatter::usedSkillCard(bool isUsed) {
    if(isUsed) {
        return "Kamu sudah menggunakan kartu kemampuan pada giliran ini!\nPenggunaan kartu dibatasi maksimal 1 kali dalam 1 giliran.\n";   
    }

    return "Kartu kemampuan hanya bisa digunakan SEBELUM melempar dadu.\n";
}

string Formatter::activateSkillCard(string name, string description) {
    return name + " diaktifkan! " + description + "\n";
}

string Formatter::effectSkillCard(string label, string description) {
    return "[" + label + "] " + description;
}

string Formatter::dropCardWarning(string name) {
    std::ostringstream oss;
    oss << "Kartu yang didapat : " << name << endl;
    oss << "PERINGATAN: Kamu sudah memiliki 3 kartu di tangan (Maksimal 3)!" << endl;
    oss << "Kamu diwajibkan membuang 1 kartu." << endl;

    return oss.str();
}

string Formatter::dropCardAction(string name) {
    return name + "telah dibuang. Sekarang kamu memiliki 3 kartu di tangan.\n";
}

string Formatter::throwException(GameException e) {
    return e.what();
}




 

