#include "utils/ConfigLoader.hpp"

const std::string ConfigLoader::configPath = "config/";
const std::string ConfigLoader::defaultPropertyFileName = "property.txt";
const std::string ConfigLoader::defaultStationFileName = "railroad.txt";
const std::string ConfigLoader::defaultUtilityFileName = "utility.txt";
const std::string ConfigLoader::defaultTaxFileName = "tax.txt";
const std::string ConfigLoader::defaultActionFileName = "aksi.txt";
const std::string ConfigLoader::defaultSpecialFileName = "special.txt";
const std::string ConfigLoader::defaultMiscFileName = "misc.txt";

std::ifstream ConfigLoader::open(std::string path){
    std::ifstream file(path);
    if (!file){
        throw FileNotFoundException(path);
    }
    return file;
}


std::map<int, int> ConfigLoader::loadIntMap(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    std::map<int, int> map;
    try{
        int key, value;
        while (file >> key >> value){
            map[key] = value;
        }
    }
    catch (const std::exception&){
        throw InvalidFileDataException();
    }
    return map;
}


std::tuple<int, int> ConfigLoader::loadTuple2(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    int num1, num2;
    try{
        file >> num1 >> num2;
    }
    catch (const std::exception&){
        throw InvalidFileDataException();
    } 
    return std::make_tuple(num1, num2);
}
    

std::tuple<int, int, int> ConfigLoader::loadTuple3(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    int num1, num2, num3;
    try{
        file >> num1 >> num2 >> num3;
    }
    catch (const std::exception&){
        throw InvalidFileDataException();
    } 
    return std::make_tuple(num1, num2, num3);
}


Color ConfigLoader::colorTypeToEnum(std::string color){
    if (color == "DEFAULT"){
        return Color::DEFAULT;
    }
    else if (color == "COKLAT"){
        return Color::BROWN;
    }
    else if (color == "BIRU_MUDA"){
        return Color::LIGHTBLUE;
    }
    else if (color == "MERAH_MUDA"){
        return Color::PINK;
    }
    else if (color == "ABU_ABU"){
        return Color::GRAY;
    }
    else if (color == "ORANGE"){
        return Color::ORANGE;
    }
    else if (color == "MERAH"){
        return Color::RED;
    }
    else if (color == "KUNING"){
        return Color::YELLOW;
    }
    else if (color == "HIJAU"){
        return Color::GREEN;
    }
    else if (color == "BIRU_TUA"){
        return Color::DARKBLUE;
    }
    else{
        throw UnknownColorException(color);
    }
}

std::vector<std::pair<int, std::unique_ptr<Plot>>> ConfigLoader::loadProperty(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    std::vector<std::pair<int, std::unique_ptr<Plot>>> tiles;
    try{
        int id, buyPrice, mortgageValue;
        std::string code, name, type, color;
        while (file >> id >> code >> name >> type >> color >> buyPrice >> mortgageValue){
            std::map<int, int> rentPriceTable;

            //Ubah color (string) menjadi enum Color
            Color colorEnum = ConfigLoader::colorTypeToEnum(color);

            //Buat plot sesuai tipenya
            std::unique_ptr<Plot> plot;

            if (type == "STREET"){
                //Baca data lainnya
                int upgHousePrice, upgHotelPrice;
                file >> upgHousePrice >> upgHotelPrice;

                //Isi rentPriceTable
                file >> id >> code >> name >> type;
                for (int i = 0; i <= 5; i++){
                    int value;
                    file >> value;
                    rentPriceTable[i] = value;
                }

                plot = std::unique_ptr<Plot>(new LandPlot(name, code, colorEnum, mortgageValue, buyPrice,
                                    upgHousePrice, upgHotelPrice, rentPriceTable, nullptr, PropertyStatus::BANK, 0, 1));

            }
            else if (type == "RAILROAD"){
                plot = std::unique_ptr<Plot>(new StationPlot(name, code, colorEnum, buyPrice, mortgageValue, nullptr, PropertyStatus::BANK, 0, 1));
            }
            else if (type == "UTILITY"){
                plot = std::unique_ptr<Plot>(new UtilityPlot(name, code, colorEnum, buyPrice, mortgageValue, nullptr, PropertyStatus::BANK, 0, 1));
            }
            else{
                throw UnknownTypeException(type);
            }

            tiles.push_back(std::make_pair(id, std::move(plot)));
        }
    }
    catch (const GameException&){
        throw;
    }
    catch (const std::exception&){
        throw InvalidFileDataException();
    } 

    return tiles;
}


std::map<int, int> ConfigLoader::loadRailroad(std::string path){
    return loadIntMap(path);
}


std::map<int, int> ConfigLoader::loadUtility(std::string path){
    return loadIntMap(path);
}

std::vector<std::pair<int, std::unique_ptr<Plot>>> ConfigLoader::loadAction(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    std::vector<std::pair<int, std::unique_ptr<Plot>>> tiles;
    try{
        int id;
        std::string code, name, type, color;
        while (file >> id >> code >> name >> type >> color){
            //Ubah color (string) menjadi enum Color
            Color colorEnum = ConfigLoader::colorTypeToEnum(color);

            //Buat plot sesuai tipenya
            std::unique_ptr<Plot> plot;

            if (type == "SPESIAL"){
                if (code == "GO"){
                    plot = std::unique_ptr<Plot>(new StartPlot(name, code, colorEnum));
                }
                else if (code == "PEN"){
                    plot = std::unique_ptr<Plot>(new PrisonPlot(name, code, colorEnum));
                }
                else if (code == "BBP"){
                    plot = std::unique_ptr<Plot>(new FreeParkPlot(name, code, colorEnum));
                }
                else if (code == "PPJ"){
                    plot = std::unique_ptr<Plot>(new GoPrisonPlot(name, code, colorEnum));
                }
                else{
                    throw UnknownTypeException(type + " dengan kode " + code);
                }
            }
            else if (type == "KARTU"){
                if (code == "DNU"){
                    plot = std::unique_ptr<Plot>(new CommunityChestCardPlot(name, code, colorEnum));
                }
                else if (code == "KSP"){
                    plot = std::unique_ptr<Plot>(new ChanceCardPlot(name, code, colorEnum));
                }
                else{
                    throw UnknownTypeException(type + " dengan kode " + code);
                }
            }
            else if (type == "PAJAK"){
                if (code == "PPH"){
                    plot = std::unique_ptr<Plot>(new IncomeTaxPlot(name, code, colorEnum));
                }
                else if (code == "PBM"){
                    plot = std::unique_ptr<Plot>(new LuxuryTaxPlot(name, code, colorEnum));
                }
                else{
                    throw UnknownTypeException(type + " dengan kode " + code);
                }
            }
            else if (type == "FESTIVAL"){
                plot = std::unique_ptr<Plot>(new FestivalPlot(name, code, colorEnum));
            }
            else{
                throw UnknownTypeException(type);
            }

            tiles.push_back(std::make_pair(id, std::move(plot)));
        }
    }
    catch (const GameException&){
        throw;
    }
    catch (const std::exception&){
        throw InvalidFileDataException();
    }

    return tiles;
}


std::tuple<int, int, int> ConfigLoader::loadTax(std::string path){
    return loadTuple3(path);
}


std::tuple<int, int> ConfigLoader::loadSpecial(std::string path){
    return loadTuple2(path);
}


std::tuple<int, int> ConfigLoader::loadMisc(std::string path){
    return loadTuple2(path);
}

std::vector<unique_ptr<ChanceCard>> ConfigLoader::loadChanceCard(){
    std::vector<unique_ptr<ChanceCard>> cards;
    cards.push_back(make_unique<ChanceCard>(BackwardMoveCard()));
    cards.push_back(make_unique<ChanceCard>(PrisonCard()));
    cards.push_back(make_unique<ChanceCard>(StationCard()));
}

std::vector<unique_ptr<CommunityChestCard>> ConfigLoader::loadCommunityChestCard(){
    std::vector<unique_ptr<CommunityChestCard>> cards;
    cards.push_back(make_unique<CommunityChestCard>(BirthdayCard()));
    cards.push_back(make_unique<CommunityChestCard>(CampaignCard()));
    cards.push_back(make_unique<CommunityChestCard>(DoctorCard()));
}

std::vector<shared_ptr<SkillCard>> ConfigLoader::loadSkillCard(){
    std::vector<shared_ptr<SkillCard>> cards;
    // MoveCard (4 lembar): Memungkinkan pemain untuk bergerak maju sekian petak. Nominal langkah pergerakan ditentukan secara acak pada saat pemain pertama kali mendapatkan kartu ini.
    for (int i = 0; i < 4; ++i) cards.push_back(make_shared<SkillCard>(MoveCard()));

    // DiscountCard (3 lembar): Memberikan diskon dengan persentase acak kepada pemain saat kartu didapatkan. Masa berlaku DiscountCard adalah 1 giliran.
    for (int i = 0; i < 3; ++i) cards.push_back(make_shared<SkillCard>(DiscountCard()));

    // ShieldCard (2 lembar): Melindungi pemain dari tagihan sewa maupun sanksi apapun yang merugikan selama 1 giliran.
    for (int i = 0; i < 2; ++i) cards.push_back(make_shared<SkillCard>(ShieldCard()));

    // TeleportCard (2 lembar): Memberikan pemain kebebasan untuk berpindah ke petak manapun di atas papan permainan.
    for (int i = 0; i < 2; ++i) cards.push_back(make_shared<SkillCard>(TeleportCard()));

    // LassoCard (2 lembar): Menarik satu pemain lawan yang berada di depan posisi pemain saat ini ke petak tempat pemain tersebut berada.
    for (int i = 0; i < 2; ++i) cards.push_back(make_shared<SkillCard>(LassoCard()));

    // DemolitionCard (2 lembar): Menghancurkan satu properti milik pemain lawan.
    for (int i = 0; i < 2; ++i) cards.push_back(make_shared<SkillCard>(DemolitionCard()));
}


