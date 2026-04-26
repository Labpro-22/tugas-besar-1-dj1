#include "core/TurnManager.hpp"

#include <string>
#include "core/GameException.hpp"
#include "core/services/BankruptcyService.hpp"
#include "core/services/CommandHandler.hpp"
#include "models/Board/Board.hpp"
#include "models/Board/CardDeck.hpp"
#include "models/Card/SkillCard/SkillCard.hpp"

void TurnManager::startTurn(Player& player, GameState& state) {
    player.resetTurnFlags();
    player.resetConsecutiveDoubles();

    if (player.getStatus() == PlayerStatus::JAILED) {
        if (player.getJailTurns() <= 0) {
            state.addLog(player.getUsername() +
                " sudah melewati batas giliran di penjara, wajib membayar denda.");
            if (!payJailFine(player, state)) {
                state.addLog(player.getUsername() +
                    " tidak dapat membayar denda penjara dan dinyatakan bangkrut.");
            }
            return;
        }

        state.addLog(player.getUsername() + " berada di penjara (" +
            std::to_string(player.getJailTurns()) +
            " giliran tersisa). Pilih: PAY_JAIL_FINE atau ROLL untuk mencoba double.");
        return;
    }

    state.addLog("Mulai giliran: " + player.getUsername());
}

void TurnManager::endTurn(Player& player, GameState& state) {
    state.addLog("Selesai giliran: " + player.getUsername());
}

bool TurnManager::handleExtraTurn(Player& player, bool rolledDouble, GameState& state) {
    if (!rolledDouble) {
        return false;
    }
    if (player.isBankrupt() || player.getStatus() == PlayerStatus::JAILED) {
        return false;
    }

    player.setHasRolled(false);
    state.addLog(player.getUsername() + " mendapat extra turn setelah melempar double.");
    return true;
}

void TurnManager::advanceTurn(GameState& state) {
    state.nextPlayer();
    if (state.getCurrentTurn() > state.getMaxTurn()) {
        state.setGameOver(true);
        state.addLog("Permainan selesai karena batas giliran tercapai.");
    }
}

void TurnManager::sendToJail(Player& player, GameState& state) {
    const int boardSize = state.getBoardSizeOrDefault();
    int jailIndex = state.getBoard().findPlotIndex("PEN");
    if (jailIndex < 0 || jailIndex >= boardSize) {
        jailIndex = 0;
    }

    try {
        player.moveTo(jailIndex, boardSize);
    } catch (const GameException&) {
        // ignore move failure — status change is the important part
    }
    player.goToJail();
    state.addLog(player.getUsername() + " masuk penjara.");
}

bool TurnManager::payJailFine(Player& player, GameState& state) {
    if (player.getStatus() != PlayerStatus::JAILED) {
        return false;
    }

    BankruptcyService bankruptcyService;
    if (!bankruptcyService.canRecover(player, JAIL_FINE)) {
        bankruptcyService.liquidateAssets(player, JAIL_FINE - player.getCash(), state);
    }
    if (!bankruptcyService.canRecover(player, JAIL_FINE)) {
        bankruptcyService.transferAssets(player, nullptr, state);
        return false;
    }

    player.pay(JAIL_FINE);
    player.setStatus(PlayerStatus::ACTIVE);
    player.setJailTurns(0);
    state.addLog(player.getUsername() + " membayar denda penjara sebesar " +
        std::to_string(JAIL_FINE) + " dan bebas.");
    return true;
}

bool TurnManager::handleJailedRoll(Player& player, bool rolledDouble, GameState& state) {
    if (player.getStatus() != PlayerStatus::JAILED) {
        return true;
    }

    if (rolledDouble) {
        player.setStatus(PlayerStatus::ACTIVE);
        player.setJailTurns(0);
        player.resetConsecutiveDoubles();
        state.addLog(player.getUsername() + " bebas dari penjara karena melempar double.");
        return true;
    }

    player.decrementJailTurns();
    if (player.getJailTurns() <= 0) {
        state.addLog(player.getUsername() +
            " gagal melempar double tiga kali. Wajib membayar denda penjara.");
        if (payJailFine(player, state)) {
            return true;
        }
        return false;
    }

    state.addLog(player.getUsername() + " gagal melempar double, tetap di penjara (" +
        std::to_string(player.getJailTurns()) + " giliran tersisa).");
    return false;
}

bool TurnManager::handleTripleDouble(Player& /*player*/, GameState& /*state*/) {
    // TODO: Implementasi deteksi "melanggar batas kecepatan" (3 double berturut-turut).
    //
    // Alur yang harus dijalankan:
    // 1. Pre-kondisi: method ini dipanggil SETELAH Dice::roll() menghasilkan double
    //    dan SEBELUM bidak pemain digerakkan berdasarkan dadu ke-3. Pastikan caller
    //    (GameEngine / command handler LEMPAR_DADU) memanggil method ini sebelum
    //    memanggil player.move() agar bidak TIDAK ikut maju ke petak hasil dadu.
    //
    // 2. Tambahkan counter double:
    //    - Panggil player.incrementConsecutiveDoubles() setiap kali hasil dadu double.
    //    - Reset dengan player.resetConsecutiveDoubles() apabila hasil dadu bukan double
    //      (lakukan di tempat pemanggilan, bukan di sini).
    //
    // 3. Cek apakah player.getConsecutiveDoubles() >= MAX_CONSECUTIVE_DOUBLES (3).
    //    - Jika belum tercapai: return false (caller boleh lanjut menggerakkan bidak).
    //    - Jika tercapai: jalankan sub-alur 4 di bawah.
    //
    // 4. Triple double tercapai:
    //    - state.addLog dengan jenis aksi "DOUBLE" / "JAIL" bahwa pemain melanggar
    //      batas kecepatan (sesuai Transaction Logger).
    //    - Panggil sendToJail(player, state). Catatan: sendToJail sudah
    //      memanggil resetConsecutiveDoubles() dan setHasRolled(true), jadi tidak
    //      perlu reset manual di sini.
    //    - Pastikan giliran pemain BERAKHIR saat itu juga (caller harus cek return
    //      value method ini untuk skip sisa alur giliran dan langsung advanceTurn).
    //    - Return true untuk menandai giliran dipotong.
    //
    // 5. Edge case:
    //    - Pemain yang sudah JAILED tidak boleh memicu triple double lagi; cek
    //      player.getStatus() == PlayerStatus::JAILED di awal dan return false.
    //    - Jangan reset consecutiveDoubles di sini jika kondisi belum tercapai;
    //      biarkan tetap agar akumulasi berjalan.
    //
    // Return: true jika pemain dikirim ke penjara akibat triple double,
    //         false jika belum tercapai atau tidak relevan.
    return false;
}

void TurnManager::grantGoSalary(Player& player, GameState& state) {
    // TODO: Pemberian gaji GO saat pemain mendarat TEPAT di petak GO.
    //
    // Alur:
    // 1. Verifikasi posisi pemain: player.getPosition() harus menunjuk ke indeks
    //    petak GO pada board. Cari indeks GO dengan state.getBoard().findPlotIndex("GO").
    //    Jika tidak cocok, early return (tidak log apa pun — tidak eligible).
    if (player.getPosition() != state.getBoard().findPlotIndex("GO")){
        return;
    }
    //
    // 2. Ambil nilai gaji dari konfigurasi: int salary = state.getSalary().
    //    Nilai ini dibaca dari special.txt (GO_SALARY) saat load config.
    //    Jangan hardcode 200.

    int salary = state.getSalary();
    //
    // 3. Tambahkan ke cash pemain: player += salary (menggunakan operator+= yang
    //    sudah dioverload di Player) atau player.receive(salary).
    //
    player += salary;
    // 4. Logging:
    //    - state.addLog(player.getUsername(), "GO", "Mendarat di GO, menerima gaji M" + salary).
    //
    state.addLog(player.getUsername(), "GO", "Mendarat di GO, menerima gaji M" + std::to_string(salary));
    // 5. Edge case:
    //    - Jangan panggil method ini saat pemain dikirim ke penjara via PPJ
    //      (spec: "pemain tidak mendapatkan gaji dari melewati Petak Mulai").
    //      Pengecekan ini harus dilakukan di caller, bukan di sini.
    //    - Method ini HANYA untuk landing tepat di GO. Untuk "melewati" GO,
    //      gunakan handlePassedGo.
    //
    // Catatan: Di monopoli kanonik, landing on GO sering hanya memberi gaji normal
    // (sama seperti passing) — sesuaikan dengan interpretasi tim. Spec tubes ini
    // mengatakan "berhenti tepat di petak ini ATAU melewati petak ini" → sama saja
    // satu kali gaji. Jika caller sudah memanggil handlePassedGo saat pergerakan,
    // method ini mungkin tidak perlu memberi gaji tambahan (duplikasi).
}

void TurnManager::handlePassedGo(Player& player, int oldPosition, int newPosition, GameState& state) {
    // TODO: Pemberian gaji GO saat pemain MELEWATI petak GO (tidak berhenti tepat).
    //
    // Kapan dipanggil: setiap kali posisi pemain berubah karena pergerakan searah
    // jarum jam (hasil dadu, kartu "Mundur 3 petak" → TIDAK maju, MoveCard,
    // TeleportCard). TeleportCard kemungkinan TIDAK memberikan gaji (mundur/teleport
    // bukan "lewat" secara fisik) — confirm ke asisten via QnA.
    //
    // Alur:
    // 1. Cari indeks GO: int goIndex = state.getBoard().findPlotIndex("GO").
    //    Biasanya goIndex == 0.
    int goIndex = state.getBoard().findPlotIndex("GO");
    int boardSize = state.getBoardSizeOrDefault();
    //
    // 2. Deteksi "melewati GO":
    //    - Board bersifat circular (modulo boardSize).
    //    - Pemain melewati GO jika pergerakan maju menyeberangi titik goIndex.
    //    - Contoh logika (jika gerakan maju / clockwise dan goIndex = 0):
    //        if (newPosition < oldPosition) -> sudah wrap melewati GO
    //      Untuk goIndex != 0, gunakan perhitungan modular.
    //    - Jangan beri gaji jika pemain MUNDUR (kartu Mundur 3 petak) meskipun
    //      melewati GO dari arah berlawanan — spec: gaji hanya saat "melewati"
    //      searah jarum jam. Cek arah pergerakan dari caller.
    if (goIndex < 0 || boardSize <= 0) return;
    if (newPosition == goIndex) return;

    int distanceToGo = (goIndex - oldPosition + boardSize) % boardSize;
    int distanceMoved = (newPosition - oldPosition + boardSize) % boardSize;

    bool passedGo = distanceToGo > 0 && distanceToGo < distanceMoved;

    if (!passedGo) {
        return;
    }
    //
    // 3. Jika kondisi melewati terpenuhi:
    //    - Berikan gaji: player.receive(state.getSalary()) atau player += salary.
    //    - Log via state.addLog(player.getUsername(), "GO", "Melewati GO, terima gaji M...").
    //
    int salary = state.getSalary();
    player += salary;

    state.addLog(player.getUsername(), "GO", "Melewati GO, menerima gaji M" + std::to_string(salary));
    // 4. Edge case penting:
    //    - Pemain dikirim ke penjara via PPJ → bidak pindah dari PPJ (indeks 30)
    //      ke PEN (indeks 10). Ini pergerakan MUNDUR, jangan beri gaji walaupun
    //      posisinya "wrap" secara numerik. Caller harus menandai konteks ini.
    //    - Pemain TEPAT berhenti di GO (newPosition == goIndex): keputusan tim
    //      apakah ini dianggap "lewat" juga atau panggil grantGoSalary saja.
    //      Hindari double-count.
    //    - Pemain JAILED tidak bergerak, method ini tidak dipicu.
    //
    // Testing: buat test case di mana pemain di posisi 38 lempar dadu 5 → mendarat
    // di posisi 3 (setelah wrap). Assert cash bertambah sebesar salary.
}

bool TurnManager::checkGameEndConditions(GameState& state) {
    // TODO: Cek semua kondisi akhir permainan secara terpusat.
    //
    // Return true jika game harus berakhir, false jika lanjut.
    //
    // Ada 2 mode game end sesuai spec:
    //
    // MODE A: MAX_TURN (normal)
    //   - Aktif ketika state.getMaxTurn() >= 1.
    //   - Trigger: state.getCurrentTurn() > state.getMaxTurn().
    //   - Action:
    //       state.setGameOver(true);
    //       state.addLog("Permainan selesai: batas giliran tercapai.");
    //     Lalu tentukan pemenang (bisa di method lain determineWinnerByMaxTurn):
    //       1. Uang terbanyak
    //       2. Jika seri -> jumlah properti terbanyak
    //       3. Jika masih seri -> jumlah kartu terbanyak
    //       4. Jika masih seri -> semua yang masih seri = pemenang bersama
    //     Gunakan operator< / operator> pada Player yang sudah dioverload.
    //
    // MODE B: BANKRUPTCY (tanpa batas)
    //   - Aktif ketika state.getMaxTurn() < 1 (invalid config → mode tanpa batas).
    //   - Trigger: state.hasSingleActivePlayer() == true (sisa 1 aktif).
    //   - Action:
    //       state.setGameOver(true);
    //       state.addLog("Permainan selesai: satu pemain tersisa.");
    //     Pemenang = satu-satunya pemain non-BANKRUPT.
    //
    // TAMBAHAN (berlaku di KEDUA mode):
    //   - Jika hanya tersisa 1 pemain aktif karena bankruptcy, game berakhir
    //     segera tanpa menunggu MAX_TURN (spec: "Jika hanya tersisa 1 pemain →
    //     pemain tersebut menang").
    //
    // Alur:
    // 1. Cek Mode B dulu: jika hasSingleActivePlayer() -> set game over, return true.
    if (state.hasSingleActivePlayer()){
        state.setGameOver(true);
        state.addLog("SYSTEM", "GAME_END", "Permainan selesai: satu pemain tersisa.");
        return true;
    }

    // 2. Cek Mode A: jika maxTurn valid & currentTurn > maxTurn -> set game over, return true.
    if (state.getCurrentTurn() > state.getMaxTurn()){
        state.setGameOver(true);
        state.addLog("SYSTEM", "GAME_END", "Permainan selesai: batas giliran tercapai.");
        return true;
    }


    // 3. Else return false.
    //
    // Catatan integrasi:
    //   - advanceTurn() harus memanggil method ini SEBELUM memulai giliran berikutnya.
    //   - Caller (GameEngine) memanggil method penentu pemenang dan render UI
    //     "MENANG (OTOMATIS)" setelah method ini return true.
    return false;
}

void TurnManager::drawSkillCardAtStart(Player& player, GameState& state, CommandHandler& /*commandHandler*/) {
    // TODO: Draw 1 kartu Skill acak untuk pemain di awal giliran.
    //
    // Spec (Aturan Kartu Kemampuan Spesial #2):
    //   "Pada awal giliran, semua pemain akan mendapatkan masing-masing 1 kartu acak."
    //   -- Ambigu: "semua pemain" vs "pemain yang bergiliran". Interpretasi
    //   umum: pemain yang sedang bergiliran. Konfirmasi ke asisten.
    //   Spec Indonesia bisa diartikan "tiap pemain setiap giliran-nya sendiri".
    //
    // Dipanggil dari startTurn() setelah pengecekan JAIL.
    //
    // Alur:
    // 1. Akses deck Skill dari GameState. Kemungkinan: state.getSkillDeck()
    //    atau via service (SkillCardService). Sesuaikan dengan arsitektur tim.
    //
    auto& rawSkillDeck = state.getBoard().getSkillCardDeckPile();
    // 2. Jika deck kosong:
    //    - Reshuffle discard pile ke deck (spec: "Apabila kartu di dalam deck
    //      habis, seluruh kartu di tempat pembuangan akan dikocok ulang").
    //    - Panggil deck.reshuffleFromDiscard() atau setara.
    //    - Jika discard pile juga kosong (awal game, belum ada yang dipakai),
    //      skip draw — log "Deck kartu kosong, tidak ada kartu didapat".
    //
    if (rawSkillDeck.drawPileSize() == 0) {
        if (rawSkillDeck.discardPileSize() == 0) {
            state.addLog(player.getUsername(), "KARTU", "Deck kartu kosong, tidak ada kartu didapat.");
            return;
        }
        rawSkillDeck.reshuffleFromDiscard();
    }
    // 3. Draw satu kartu:
    //    auto card = deck.drawRandom();
    //
    auto rawCard = rawSkillDeck.draw();
    std::shared_ptr<SkillCard> card(rawCard, [](SkillCard*){});
    // 4. Tentukan nilai kartu saat pertama didapat (spec):
    //    - MoveCard: nilai langkah diacak saat didapat (1–6 atau range tim).
    //    - DiscountCard: persentase diskon diacak (10–50% atau sesuai tim).
    //    - ShieldCard, TeleportCard, LassoCard, DemolitionCard: tidak ada nilai.
    //    Gunakan card->roll() / card->initialize() atau setter sesuai desain.
    //
    // 5. Tambahkan ke tangan pemain: player.addOwnedCard(card).
    //
    player.addOwnedCard(card);
    state.addLog(player.getUsername(), "KARTU", "Mendapat kartu " + rawCard->getName());
    // 6. Cek overflow (tangan jadi > 3):
    //    if (player.getOwnedCards().size() > 3) {
    //        handleCardOverflow(player, state);
    //    }
    //    Lihat TODO handleCardOverflow untuk alur drop.
    //
    if (player.getOwnedCards().size() > 3){
        // TODO: handleCardOverflow(player, state, commandHandler, rawSkillDeck);
    }
    // 7. Logging:
    //    state.addLog(player.getUsername(), "KARTU", "Mendapat kartu " + card->getName());
    //
    state.addLog(player.getUsername(), "KARTU", "Mendapat kartu " + rawCard->getName());
    // 8. Edge case:
    //    - Pemain JAILED: spec tidak eksplisit melarang, tapi tidak logis jika
    //      tidak bisa pakai kartu (hanya boleh sebelum lempar dadu, sedangkan
    //      di penjara bisa memilih roll). Default: tetap draw, biar mereka
    //      bisa akumulasi.
    //    - Pemain BANKRUPT: SKIP draw.
    //    - Giliran ekstra (extra turn): JANGAN draw lagi — kartu hanya di awal
    //      giliran "baru", bukan giliran ekstra dari double. Caller harus
    //      memastikan drawSkillCardAtStart hanya dipanggil dari startTurn
    //      pertama kali, bukan saat extra turn.
}

bool TurnManager::handleCardOverflow(Player& player, GameState& state, CommandHandler& commandHandler, CardDeck<std::shared_ptr<SkillCard>>& deck) {
    // TODO: Paksa pemain drop 1 kartu saat tangannya > 3 (kartu ke-4 masuk).
    //
    // Spec (Operasi 21 "DROP KARTU KEMAMPUAN OTOMATIS"):
    //   Dipicu otomatis saat pemain menerima kartu ke-4. Wajib buang 1.
    //
    // Alur:
    // 1. Guard:
    //    if (player.getOwnedCards().size() <= 3) return false;
    //
    if (player.getOwnedCards().size() <= 3) {
        return false;
    }
    // TODO: 2. Tampilkan daftar kartu (ini UI Layer concern — pisahkan):
    //    - Idealnya method ini hanya mengembalikan "butuh drop" (return true)
    //      dan UI Layer / CommandHandler yang menampilkan prompt + membaca input.
    //    - Atau, terima callback/strategy dari caller untuk memilih kartu
    //      (agar unit-testable & tidak bergantung cin).
    //

    std::size_t cardIdx = commandHandler.promptCardDrop(player);

    // 3. Setelah user memilih index (0-based atau 1-based sesuai UI):
    //    player.dropCard(cardIndex);
    //    state.addLog(player.getUsername(), "KARTU",
    //        "Membuang kartu " + droppedCardName + " (tangan penuh)");
    //
    std::string droppedName = player.getOwnedCards()[cardIdx]->getName();
    player.dropCard(cardIdx, deck);
    state.addLog(player.getUsername(), "KARTU", "Membuang kartu " + droppedName + " (tangan penuh)");


    // 4. Return true menandakan drop dilakukan.
    //
    // Alternatif desain (RECOMMENDED): pisahkan menjadi dua method:
    //   - bool needsCardDrop(Player&) const -> pure check
    //   - void promptCardDrop(Player&, GameState&) -> UI + state mutation
    // Method ini saat ini adalah gabungan; refactor jika perlu.
    //
    // Edge case:
    //   - Spec: "kartu bersifat sekali pakai. Setelah dipakai, kartu akan
    //     dibuang dan diletakkan di tempat pembuangan." Kartu yang di-drop
    //     via overflow juga harus masuk discard pile agar bisa di-reshuffle
    //     nanti. Pastikan Player::dropCard() memindahkan ke discard, bukan
    //     sekadar delete.
    //   - Jika pemain membatalkan (pilih 0), spec TIDAK mengizinkan — drop
    //     WAJIB. UI harus loop sampai valid.
    return true;
}

// bool TurnManager::useJailFreeCard(Player& /*player*/, GameState& /*state*/) {
//     // TODO: Opsi keluar penjara via kartu "Bebas dari Penjara".
//     //
//     // Spec (Petak Penjara, opsi #ii):
//     //   "Menggunakan kartu 'Bebas dari Penjara' (jika punya)."
//     //
//     // CATATAN: Cek desain tim — kartu "Bebas dari Penjara" mungkin TIDAK ada
//     // di daftar Kartu Kemampuan Spesial (MoveCard, ShieldCard, dll.) maupun
//     // di Kartu Kesempatan/Dana Umum eksplisit. Spec monopoli asli menaruh kartu
//     // ini di Chance/Community Chest. Di Nimonspoli, spec daftar kartu tidak
//     // menyertakannya secara eksplisit. Konfirmasi dulu ke asisten apakah:
//     //   (a) Kartu ini tidak ada di Nimonspoli (maka method ini tidak perlu).
//     //   (b) Kartu ini ada tapi belum terdaftar di spec (tambah sendiri).
//     //
//     // Jika opsi (b) dipilih:
//     //
//     // Alur:
//     // 1. Guard: player.getStatus() == PlayerStatus::JAILED, else return false.
//     //
//     // 2. Cek kepemilikan kartu:
//     //    - Iterasi player.getOwnedCards(), cari yang bertipe JailFreeCard.
//     //    - Jika tidak ada, return false (UI bisa menampilkan "tidak punya kartu").
//     //
//     // 3. Pakai kartu:
//     //    - Panggil card->use(skillContext) atau setara.
//     //    - Kartu masuk discard pile.
//     //    - player.setUsedSkillThisTurn(true)? Keputusan desain:
//     //      apakah kartu ini dihitung sebagai penggunaan skill giliran ini
//     //      (sehingga tidak bisa pakai kartu lain)? Spec Jail tidak eksplisit.
//     //      Rekomendasi: YES, counted as skill usage.
//     //
//     // 4. Bebaskan pemain:
//     //    player.setStatus(PlayerStatus::ACTIVE);
//     //    player.setJailTurns(0);
//     //    player.resetConsecutiveDoubles();
//     //    state.addLog(player.getUsername() +
//     //        " bebas dari penjara menggunakan kartu Bebas dari Penjara.");
//     //
//     // 5. Return true → caller lanjut ke alur normal (pemain bisa lempar dadu).
//     //
//     // Edge case:
//     //   - Pemain punya >1 kartu bebas penjara: pakai yang pertama.
//     //   - Pemain sudah di giliran ke-4 dan wajib bayar: kartu tetap bisa dipakai
//     //     sebelum payment forced. Spec tidak melarang.
//     return false;
// }

// int TurnManager::getJailFineFromConfig(const GameState& state) const {
//     // TODO: Helper untuk mengambil jumlah denda penjara dari config, bukan
//     // konstanta hardcoded.
//     //
//     // Masalah saat ini: TurnManager::JAIL_FINE = 50 hardcoded sebagai
//     // static constexpr. Spec menyuruh baca dari special.txt (JAIL_FINE).
//     //
//     // Alur:
//     // 1. Return state.getJailFine() yang sudah di-populate oleh config loader
//     //    dari special.txt.
//     //
//     // 2. Fallback: jika state.getJailFine() <= 0 (config belum di-load),
//     //    return TurnManager::JAIL_FINE sebagai default aman.
//     //
//     // Contoh:
//     //   int configFine = state.getJailFine();
//     //   return configFine > 0 ? configFine : JAIL_FINE;
//     //
//     // 3. Refactor: payJailFine() saat ini menggunakan JAIL_FINE langsung. Ganti
//     //    pemanggilan tersebut dengan getJailFineFromConfig(state) agar selalu
//     //    konsisten dengan config. Lakukan di:
//     //      - payJailFine (sisa method yang memakai JAIL_FINE literal)
//     //      - startTurn (log "wajib membayar denda" saat jailTurns habis; jika
//     //        ada string bernilai 50 hardcoded, ganti juga)
//     //
//     // 4. Konstanta JAIL_FINE di hpp bisa tetap ada sebagai fallback, tetapi
//     //    jangan jadi source of truth.
//     (void)state;
//     return JAIL_FINE;
// }
