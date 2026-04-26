#pragma once

#include <exception>
#include <string>

enum class GameErrorID{
    ILLEGALFUNCTIONCALLEXCEPTION,
    INVALIDINPUTEXCEPTION,
    INVALIDSTATEEXCEPTION,
    OUTOFRANGEEXCEPTION,
    FILENOTFOUNDEXCEPTION,
    INVALIDFILEDATAEXCEPTION,
    UNKNOWNTYPEEXCEPTION,
    UNKNOWNCOLOREXCEPTION,
    INSUFFICIENTFUNDEXCEPTION,
    INVALIDPLOTTYPEEXCEPTION,
    BUILDINGISFULLEXCEPTION,
    BUILDINGISEMPTYEXCEPTION,
    COLORSETNOTOWNEDEXCEPTION,
    NOACCESSTOPROPERTYEXCEPTION,
    POSITIONNOTINBOARDEXCEPTION,
    CANTDOACTIONTHISTURNEXCEPTION,
    NOCARDFOUNDEXCEPTION
};

class GameException : public std::exception{
private:
    GameErrorID errorID;
    std::string errorMsg;

public:
    GameException(GameErrorID errorID, std::string errorMsg):
        errorID(errorID), errorMsg(errorMsg) {}

    int getID() const {
        return static_cast<int>(errorID);
    }

    const char* what() const noexcept override {
        return errorMsg.c_str();
    }
};


class IllegalFunctionCallException : public GameException{
public:
    IllegalFunctionCallException(std::string warning): GameException(GameErrorID::ILLEGALFUNCTIONCALLEXCEPTION,
        "Terjadi error dalam pemanggilan fungsi: " + warning) {}
};

class InvalidInputException : public GameException{
public:
    InvalidInputException(): GameException(GameErrorID::INVALIDINPUTEXCEPTION,
        "Input tidak valid.") {}

    explicit InvalidInputException(const std::string& msg): GameException(
        GameErrorID::INVALIDINPUTEXCEPTION, msg) {}
};

class InvalidStateException : public GameException{
public:
    InvalidStateException(): GameException(GameErrorID::INVALIDSTATEEXCEPTION,
        "State permainan tidak valid.") {}

    explicit InvalidStateException(const std::string& msg): GameException(
        GameErrorID::INVALIDSTATEEXCEPTION, msg) {}
};

class OutOfRangeException : public GameException{
public:
    OutOfRangeException(): GameException(GameErrorID::OUTOFRANGEEXCEPTION,
        "Nilai berada di luar rentang yang valid.") {}

    explicit OutOfRangeException(const std::string& msg): GameException(
        GameErrorID::OUTOFRANGEEXCEPTION, msg) {}
};

class FileNotFoundException : public GameException{
public:
    FileNotFoundException(std::string path): GameException(GameErrorID::FILENOTFOUNDEXCEPTION,
        "Tidak ditemukan file pada path: " + path) {}
};

class InvalidFileDataException : public GameException{
public:
    InvalidFileDataException(): GameException(GameErrorID::INVALIDFILEDATAEXCEPTION,
        "Terdapat data yang tidak valid dalam file.") {}
};

class UnknownTypeException : public GameException{
public:
    UnknownTypeException(std::string type): GameException(GameErrorID::UNKNOWNTYPEEXCEPTION,
        "Jenis property " + type + " tidak dikenali oleh program.") {}
};

class UnknownColorException : public GameException{
public:
    UnknownColorException(std::string color): GameException(GameErrorID::UNKNOWNCOLOREXCEPTION,
        "Warna " + color + " tidak dikenali oleh program.") {}
};

class InsufficientFundException : public GameException{
public:
    InsufficientFundException(): GameException(GameErrorID::INSUFFICIENTFUNDEXCEPTION,
        "Uang tidak cukup untuk melakukan aksi ini.") {}
};

class InvalidPlotTypeException: public GameException{
public:
    InvalidPlotTypeException(): GameException(GameErrorID::INVALIDPLOTTYPEEXCEPTION,
        "Tidak bisa melakukan aksi pada jenis plot ini.") {}
};

class BuildingIsFullException : public GameException{
public:
    BuildingIsFullException(): GameException(GameErrorID::BUILDINGISFULLEXCEPTION,
        "Gagal membangun, bangunan sudah mencapai level maksimum.") {}
};

class BuildingIsEmptyException : public GameException{
public:
    BuildingIsEmptyException(): GameException(GameErrorID::BUILDINGISEMPTYEXCEPTION,
        "Tidak ada bangunan yang bisa dijual pada petak ini.") {}
};

class ColorSetNotOwnedException : public GameException{
public:
    ColorSetNotOwnedException(): GameException(GameErrorID::COLORSETNOTOWNEDEXCEPTION,
        "Dapatkan semua petak dengan warna sama agar bisa membangun.") {}
};

class NoAccessToPropertyException : public GameException{
public:
    NoAccessToPropertyException(): GameException(GameErrorID::NOACCESSTOPROPERTYEXCEPTION,
        "Anda tidak memiliki akses untuk properti ini.") {}
};

class PositionNotInBoardException : public GameException{
public:
    PositionNotInBoardException(int index): GameException(GameErrorID::POSITIONNOTINBOARDEXCEPTION,
        "Petak dengan nomor " + std::to_string(index) + " tidak ada di dalam board.") {}
};

class CantDoActionThisTurnException : public GameException{
public:
    CantDoActionThisTurnException(std::string action): GameException(GameErrorID::CANTDOACTIONTHISTURNEXCEPTION,
        "Tidak bisa " + action + " di turn ini.") {}
};

class NoCardFoundException : public GameException{
public:
    NoCardFoundException(): GameException(GameErrorID::NOCARDFOUNDEXCEPTION,
        "Kartu tidak ditemukan.") {}
};