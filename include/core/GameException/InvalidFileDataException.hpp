#pragma once

#include "core/GameException/GameException.hpp"

class InvalidFileDataException : public GameException{
public:
    InvalidFileDataException(std::string invalidData): GameException(GameErrorID::INVALIDFILEDATAEXCEPTION,
        "Terdapat data yang tidak valid dalam file: " + invalidData) {}
};