# Tugas Besar 1 IF2010 Pemrograman Berorientasi Objek ~ Nimonspoli — CLI Monopoly Game

**Nimonspoli** adalah permainan papan simulasi perdagangan properti berbasis *Command-Line Interface* (CLI) yang ditulis dalam **C++17**. Proyek ini dikembangkan untuk mata kuliah **IF2010 — Pemrograman Berorientasi Objek (Tubes 1)** dengan fokus pada penerapan prinsip-prinsip OOP: *Encapsulation*, *Inheritance*, *Polymorphism*, dan *Abstraction*.

<img width="1920" height="1080" alt="domtracer (1)" src="https://github.com/user-attachments/assets/effccd3e-3bd0-4e0d-b378-03f114a58e40" />

---

## Daftar Isi

1. [Fitur Utama](#-fitur-utama)
2. [Persyaratan Sistem](#%EF%B8%8F-persyaratan-sistem)
3. [Instalasi & Build](#-instalasi--build)
4. [Menjalankan Permainan](#-menjalankan-permainan)
5. [Alur Permainan](#-alur-permainan)
6. [Daftar Perintah](#-daftar-perintah)
7. [Petak (Plot) di Papan](#-petak-plot-di-papan)
8. [Sistem Kartu](#-sistem-kartu)
9. [Mekanika Khusus](#%EF%B8%8F-mekanika-khusus)
10. [Format File Konfigurasi](#-format-file-konfigurasi)
11. [Save & Load](#-save--load)
12. [Struktur Proyek](#-struktur-proyek)
13. [Arsitektur & Desain](#%EF%B8%8F-arsitektur--desain)
14. [Troubleshooting](#-troubleshooting)
15. [Kredit](#-kredit)

---

## Fitur Utama

- **Gameplay Klasik Monopoly** — pergerakan dadu, jual-beli properti, sewa, pajak, penjara, dan kondisi kemenangan.
- **Tiga Jenis Properti** — *Street* (dengan rumah & hotel), *Railroad* (sewa progresif), dan *Utility* (sewa berdasarkan dadu).
- **Sistem Ekonomi Lengkap** — gadai/tebus, lelang otomatis ketika pemain menolak membeli, monopoli warna untuk syarat membangun, dan likuidasi aset saat hampir bangkrut.
- **Tiga Jenis Kartu** — *Chance Card*, *Community Chest Card*, dan *Skill Card* yang bisa disimpan & digunakan sebelum melempar dadu.
- **Penjara dengan Aturan Lengkap** — masuk via petak `Ke_Penjara`/kartu/triple-double, keluar via dadu *double*, bayar denda, atau habisnya batas giliran.
- **Save / Load** — simpan progres permainan ke `data/<nama>.txt` lalu muat kembali kapan saja.
- **Transaction Logger** — semua aksi penting tercatat dan dapat ditampilkan dengan `CETAK_LOG`.
- **Multi-Player** — mendukung 2–4 pemain dalam satu sesi.

---

## Persyaratan Sistem

| Komponen   | Minimum                                            |
| ---------- | -------------------------------------------------- |
| Compiler   | GCC / G++ dengan dukungan **C++17**                |
| Build Tool | `make` (Linux/macOS/WSL) atau `mingw32-make` (Win) |
| OS         | Linux, macOS, atau Windows (native + WSL)          |
| Terminal   | Mendukung ANSI escape code (warna)                 |

> **Catatan Windows**: Disarankan menggunakan terminal yang mendukung warna ANSI (Windows Terminal, PowerShell 7, atau Git Bash) untuk pengalaman tampilan terbaik.

---

## Instalasi & Build

### 1. Klona Repositori

```bash
git clone <repository-url>
cd tugas-besar-1-dj1
```

### 2. Build

**Linux / macOS / WSL:**

```bash
make
```

**Windows (MinGW):**

```powershell
mingw32-make
```

Hasil kompilasi akan tersedia di `bin/game` (atau `bin/game.exe` di Windows). File objek perantara berada di `build/`.

### 3. Membersihkan Build

```bash
make clean        # Linux/macOS/WSL
mingw32-make clean # Windows
```

---

## Menjalankan Permainan

```bash
./bin/game           # Linux/macOS/WSL
bin\game.exe         # Windows
```

Saat pertama kali dijalankan, program akan menanyakan:

1. Apakah ingin **memuat permainan tersimpan**? (`y/n`)
2. Jika *baru*: jumlah pemain (**2–4**) dan nama setiap pemain.
3. Permainan dimulai dengan papan yang ditampilkan dan giliran berputar.

---

## Alur Permainan

```
┌──────────────┐
│  Mulai Turn  │ ── tarik 1 Skill Card otomatis
└──────┬───────┘
       │
       ▼
┌─────────────────────────┐
│  Sebelum lempar dadu:   │
│  • GUNAKAN_KEMAMPUAN    │
│  • GADAI / TEBUS        │
│  • BANGUN               │
│  • SIMPAN / CETAK_*     │
└──────┬──────────────────┘
       │
       ▼
┌──────────────────┐
│  LEMPAR_DADU     │ → bergerak → trigger startEvent petak tujuan
└──────┬───────────┘
       │
       ▼
┌──────────────────────────┐
│  Resolve Landing:        │
│  • Beli/Sewa properti    │
│  • Bayar pajak           │
│  • Tarik Chance/Dana Umum│
│  • Festival, Penjara, dll│
└──────┬───────────────────┘
       │
       ▼
   (Double? ulang LEMPAR_DADU; Triple double → Penjara)
       │
       ▼
┌──────────────┐
│     END      │ → giliran pindah ke pemain berikutnya
└──────────────┘
```

Kondisi permainan selesai:

- **Single Survivor** — hanya satu pemain yang belum bangkrut.
- **MAX TURN** — total giliran melampaui batas dari `config/misc.txt`. Pemenang ditentukan dari total kekayaan (cash + nilai aset).

---

## Daftar Perintah

Semua perintah **tidak peka huruf besar/kecil** dan menerima alias bahasa Inggris.

### Tampilan & Info

| Perintah                | Alias         | Deskripsi                                      |
| ----------------------- | ------------- | ---------------------------------------------- |
| `CETAK_PAPAN`           | `PRINT_BOARD` | Tampilkan papan + posisi semua pemain          |
| `CETAK_AKTA <KODE>`     | `PRINT_DEED`  | Tampilkan akta properti (mis. `CETAK_AKTA KSB`)|
| `CETAK_PROPERTI`        | `PRINT_PROPERTY` | Daftar properti milik pemain aktif          |
| `CETAK_LOG [N]`         | —             | Tampilkan N entri log terakhir (kosong = semua)|
| `HELP`                  | —             | Daftar perintah                                |

### Giliran & Dadu

| Perintah               | Alias                      | Deskripsi                              |
| ---------------------- | -------------------------- | -------------------------------------- |
| `LEMPAR_DADU`          | `ROLL`                     | Lempar dadu acak dan bergerak          |
| `ATUR_DADU <X> <Y>`    | `SETDICE`, `SET_DICE`      | Atur hasil dadu manual (X,Y ∈ [1..6])  |
| `END`                  | —                          | Akhiri giliran (wajib sudah `ROLL`)    |

### Properti

| Perintah | Alias       | Deskripsi                                                    |
| -------- | ----------- | ------------------------------------------------------------ |
| `GADAI`  | `MORTGAGE`  | Gadaikan properti (level 0) ke bank                          |
| `TEBUS`  | `REDEEM`    | Tebus properti yang sedang digadaikan                        |
| `BANGUN` | `BUILD`     | Bangun rumah / upgrade hotel (syarat: monopoli warna penuh)  |

### Kartu Kemampuan

| Perintah                  | Alias        | Deskripsi                                          |
| ------------------------- | ------------ | -------------------------------------------------- |
| `GUNAKAN_KEMAMPUAN [N]`   | `USE_SKILL`  | Aktifkan kartu ke-N (kosong = pilih interaktif)    |

> Kartu kemampuan **hanya dapat digunakan sebelum `LEMPAR_DADU`** dan **maksimal sekali per giliran**.

### Penjara

| Perintah       | Alias                                    | Deskripsi                                |
| -------------- | ---------------------------------------- | ---------------------------------------- |
| `BAYAR_DENDA`  | `PAY_JAIL_FINE`, `PAY_FINE`, `PAYJAIL`   | Bayar denda untuk keluar dari penjara    |

### Save

| Perintah               | Alias  | Deskripsi                                 |
| ---------------------- | ------ | ----------------------------------------- |
| `SIMPAN <NAMA_FILE>`   | `SAVE` | Simpan state ke `data/<nama>.txt`         |

### Perintah Otomatis (tidak perlu diketik)

`BELI`, `BAYAR_SEWA`, `BAYAR_PAJAK`, `LELANG`, `FESTIVAL`, `BANGKRUT`, `MENANG`, dan tarikan **Kesempatan / Dana Umum** dipicu oleh engine ketika kondisi terpenuhi.

---

## Petak (Plot) di Papan

Hierarki kelas petak:

```
Plot (abstract)
├── PropertyPlot
│   ├── LandPlot      (Street — bisa rumah & hotel)
│   ├── StationPlot   (Railroad — sewa progresif)
│   └── UtilityPlot   (PLN/PAM — sewa = pengali × dadu)
├── ActionPlot
│   ├── CardPlot
│   │   ├── ChanceCardPlot         (KSP — Kesempatan)
│   │   └── CommunityChestCardPlot (DNU — Dana Umum)
│   ├── FestivalPlot
│   ├── TaxPlot
│   │   ├── IncomeTaxPlot   (PPH)
│   │   └── LuxuryTaxPlot   (PBM)
└── SpecialPlot
    ├── StartPlot     (GO)
    ├── PrisonPlot    (PEN)
    ├── GoPrisonPlot  (PPJ — kirim ke penjara)
    └── FreeParkPlot  (BBP)
```

Konfigurasi default papan: **40 petak**, kombinasi 22 *Street*, 4 *Railroad*, 2 *Utility*, 2 *Tax*, 3 *Chance*, 3 *Community Chest*, 4 *Special*.

---

## Sistem Kartu

### Skill Card (kartu kemampuan spesial)

Diambil otomatis 1 buah di awal setiap giliran. Maksimum **3 kartu** di tangan; kelebihan harus dibuang.

| Kartu             | Efek                                                                |
| ----------------- | ------------------------------------------------------------------- |
| `MoveCard`        | Maju **N** petak (N acak 1–12, ditetapkan saat kartu didapat)       |
| `TeleportCard`    | Pilih petak tujuan secara bebas                                     |
| `ShieldCard`      | Lindungi dari satu efek negatif (sewa, pajak, kartu)                |
| `DiscountCard`    | Diskon harga properti / pembangunan                                 |
| `LassoCard`       | Klaim properti yang belum dimiliki dengan harga lelang              |
| `DemolitionCard`  | Hancurkan satu bangunan milik lawan                                 |

### Chance Card (Kesempatan — KSP)

| Kartu                | Efek                                                  |
| -------------------- | ----------------------------------------------------- |
| `BackwardMoveCard`   | Mundur 3 petak (memicu *startEvent* petak tujuan)     |
| `StationCard`        | Pindah ke stasiun terdekat (memicu *startEvent*)      |
| `PrisonCard`         | Langsung dipenjara 3 giliran                          |

### Community Chest Card (Dana Umum — DNU)

| Kartu           | Efek                                                  |
| --------------- | ----------------------------------------------------- |
| `BirthdayCard`  | Terima M100 dari setiap pemain lain                   |
| `CampaignCard`  | Bayar M200 ke setiap pemain lain                      |
| `DoctorCard`    | Bayar M700 ke bank                                    |

> Kartu pergerakan (`MoveCard`, `TeleportCard`, `BackwardMoveCard`, `StationCard`) **secara otomatis memicu `startEvent`** pada petak tujuan — sehingga jika mendarat di properti orang lain, sewa langsung dibayarkan.

---

## Mekanika Khusus

### Dadu & Double

- Dua dadu sisi enam.
- **Double** (kedua dadu sama) → giliran tambahan setelah resolve landing.
- **Triple double** berturut-turut → langsung masuk penjara.

### Penjara

- Maksimum **3 giliran** di penjara.
- Cara keluar: lempar **double**, bayar **denda** (default M50), atau habis batas giliran (otomatis bayar denda — bangkrut bila tidak mampu).

### Monopoli & Pembangunan

- Syarat `BANGUN`: pemain menguasai **seluruh** *Street* dalam satu *color group* dan tidak ada yang digadaikan.
- Pembangunan harus **merata** — petak baru bisa di-upgrade hanya bila levelnya = level minimum di group.
- Level: 1–4 = rumah, 5 = hotel.

### Sewa

- **Street**: dasar / 2× dasar (monopoli tanpa bangunan) / sesuai level rumah-hotel.
- **Railroad**: pengali × jumlah railroad yang dimiliki owner (`config/railroad.txt`).
- **Utility**: pengali × hasil dadu pemain yang mendarat (`config/utility.txt`).

### Lelang

- Dipicu otomatis saat pemain **menolak** membeli properti yang belum bertuan.
- Semua pemain non-bangkrut ikut bidding hingga harga mentok.

### Festival

- Petak `Festival` (jika ada di papan) memberi efek bonus/penalti khusus pada properti milik pemain di group warna tertentu.

### Bangkrut & Likuidasi

1. Pemain pertama dibantu *liquidate aset* (gadai otomatis) bila masih bisa diselamatkan.
2. Bila tetap kurang → **BANGKRUT**. Asetnya:
   - Diserahkan ke kreditor (bila ada),
   - Atau dilelang ke pemain tersisa (bila kreditor = bank/null).

---

## Format File Konfigurasi

Semua konfigurasi berada di folder `config/`. Format umum: satu *record* per baris, *whitespace-separated*.

### `config/aksi.txt` — Petak aksi & spesial

```
<index> <kode> <nama> <kategori> <subtype>
```

Contoh:

```
0  GO   Start             SPESIAL DEFAULT
4  PPH  Pajak_Penghasilan PAJAK   DEFAULT
7  KSP  Kesempatan_1      KARTU   DEFAULT
10 PEN  Penjara           SPESIAL DEFAULT
30 PPJ  Ke_Penjara        SPESIAL DEFAULT
38 PBM  Pajak_Barang_Mewah PAJAK  DEFAULT
```

### `config/property.txt` — Petak properti

Setiap properti memiliki **dua baris**: header & detail.

**Street** (header lalu detail level 0–5):

```
<index> <kode> <nama> STREET <warna> <buyPrice> <mortgage> <upHouse> <upHotel>
<index> <kode> <nama> STREET <rentLv0> <rentLv1> <rentLv2> <rentLv3> <rentLv4> <rentLv5>
```

**Railroad** (satu baris):

```
<index> <kode> <nama> RAILROAD DEFAULT <buyPrice> <mortgage>
```

**Utility** (satu baris):

```
<index> <kode> <nama> UTILITY DEFAULT <buyPrice> <mortgage>
```

### `config/railroad.txt` — Multiplier sewa railroad

```
<jumlahDimiliki> <multiplier>
```

### `config/utility.txt` — Multiplier sewa utility

```
<jumlahDimiliki> <multiplier>   # multiplier × hasil dadu
```

### `config/tax.txt` — Pajak

```
<flatPPH> <persenPPH> <flatPBM>
```

### `config/special.txt` — Gaji & denda

```
<gajiGO> <dendaPenjara>
```

### `config/misc.txt` — Aturan umum

```
<maxTurn> <startingCash>
```

---

## Save & Load

- **Simpan**: `SIMPAN <nama>.txt` (hanya **sebelum** `LEMPAR_DADU`).
- File tersimpan di folder `data/` sebagai teks ASCII, berisi snapshot lengkap (papan, pemain, kartu di tangan, deck, dll).
- **Load**: dipilih saat program start (`Apakah anda ingin memuat permainan tersimpan? y`).
- Bila file timpa: program akan meminta konfirmasi.

---

## Struktur Proyek

```
tugas-besar-1-dj1/
├── bin/                         # Executable hasil build
├── build/                       # File objek (.o)
├── config/                      # Konfigurasi default papan & ekonomi
│   ├── aksi.txt
│   ├── property.txt
│   ├── railroad.txt
│   ├── utility.txt
│   ├── tax.txt
│   ├── special.txt
│   └── misc.txt
├── data/                        # File save permainan
├── docs/                        # Dokumentasi tambahan
├── include/                     # Header files (.hpp)
│   ├── core/                    # Engine, GameState, Command, Services
│   │   └── services/            # AuctionService, BankruptcyService, EffectResolver, CommandHandler
│   ├── models/
│   │   ├── Board/               # Board, Dice, CardDeck
│   │   ├── Player/              # Player, PlayerStatus
│   │   ├── Plot/                # Plot hierarchy
│   │   │   ├── ActionPlot/      # Card / Festival / Tax plots
│   │   │   ├── PropertyPlot/    # Land / Station / Utility
│   │   │   └── SpecialPlot/     # Start / Prison / GoPrison / FreePark
│   │   └── Card/
│   │       ├── ChanceCard/
│   │       ├── CommunityChestCard/
│   │       └── SkillCard/
│   ├── utils/                   # Logger, ConfigLoader/Saver, SaveLoader
│   └── views/                   # GameRenderer, Formatter
├── src/                         # Implementation files (.cpp), mirror struktur include/
├── makefile
└── README.md
```

---

## Arsitektur & Desain

### Lapisan utama

| Lapisan          | Tanggung Jawab                                                         |
| ---------------- | ---------------------------------------------------------------------- |
| **Core / Engine**| `GameEngine`, `GameState`, `TurnManager`, `Command` — orkestrasi turn, parsing perintah, lifecycle. |
| **Services**     | `EffectResolver`, `AuctionService`, `BankruptcyService`, `CommandHandler` — logika game cross-cutting. |
| **Models**       | `Player`, `Board`, `Dice`, `Plot` & turunannya, `Card` & turunannya — representasi data domain. |
| **Views**        | `GameRenderer`, `Formatter` — semua I/O layar (warna, layout, prompt). |
| **Utils**        | `Logger`, `ConfigLoader`, `ConfigSaver`, `SaveLoader` — persistensi & utilitas. |

### Pola yang digunakan

- **Command Pattern** — setiap input pemain dimapping ke subclass `Command` (`RollDiceCommand`, `BuildCommand`, `UseSkillCardCommand`, dll). `CommandHandler` melakukan parsing string → `Command`, lalu `Command::execute(state, effectResolver, turnManager)` dijalankan.
- **Strategy / Polymorphism** — `Plot::startEvent(PlotContext&)` adalah pure virtual; setiap petak mengimplementasikan reaksinya sendiri ketika pemain mendarat.
- **Context Objects** — `PlotContext` & `SkillContext` membungkus referensi ke pemain, papan, logger, dan service yang dibutuhkan kartu/petak — menghindari ketergantungan langsung ke `GameState`.
- **Inheritance Tree** — `Plot`, `Card`, dan turunannya membentuk hierarki yang stabil dengan ekstensibilitas tinggi (tambah jenis kartu/petak baru tanpa mengubah engine).

### Prinsip OOP yang ditekankan

- **Encapsulation** — semua atribut data privat, akses via getter/setter.
- **Inheritance** — hierarki `Plot`, `Card`, `PropertyPlot`, dll.
- **Polymorphism** — `startEvent`, `activate`, `getName` di-override; engine bekerja melalui pointer ke base class.
- **Abstraction** — `Plot` & `Card` adalah abstract base; engine hanya bergantung pada antarmuka.

---

## Troubleshooting

| Masalah                                               | Solusi                                                                       |
| ----------------------------------------------------- | ---------------------------------------------------------------------------- |
| `make: command not found`                             | Install build-essentials (Linux) / MSYS2 + MinGW (Windows).                  |
| Link error `cannot open output file bin/game.exe`     | Tutup instance game yang masih jalan, lalu rebuild.                          |
| Warna ANSI muncul sebagai escape sequence aneh        | Gunakan terminal yang mendukung warna (Windows Terminal, Git Bash, iTerm).   |
| `Gagal memuat file save`                              | Pastikan file ada di folder `data/` dan formatnya tidak rusak.               |
| Prompt input dadu/kartu menolak input                 | Input harus berupa angka tanpa karakter lain. Tekan `0` untuk batal pilihan. |

---

## Kredit

Dibuat untuk pemenuhan **Tugas Besar 1 — IF2010 Pemrograman Berorientasi Objek**, Institut Teknologi Bandung.

## Deadliners J-1

**Anggota Kelompok**

| No | Nama                         | NIM       |
|----|------------------------------|-----------|
| 1  | Muhammad Ashkar              | 13524007  |
| 2  | Tengku Naufal Saqib          | 13524012  |
| 3  | Rhenaldy Cahyadi Putra       | 13524039  |
| 4  | Fayyaz Akmal Lauda           | 13524076  |
| 5  | Muhammad Faiz Alfada Dharma  | 13524097  |

Lihat `docs/` untuk dokumentasi rancangan & spesifikasi resmi.
