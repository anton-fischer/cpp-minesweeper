#include "core/settings.h"

#include <assert.h>

// TODO maybe introduce enum as return value instead of bool for better exception handling
bool Settings::setSettings(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount) {
    //if (boardSizeX < 3 || boardSizeY < 3 || boardSizeX > 10 || boardSizeY > 10) return false; // minimum boardSize is 3x3, maximum boardSize is 10x10
    //if (bombCount < 1 || bombCount >= boardSizeX * boardSizeY / 2) return false; // minimum bombCount is 1, maximum bombCount is boardSizeX * boardSizeY / 2

    this->boardSizeX = boardSizeX;
    this->boardSizeY = boardSizeY;
    this->bombCount = bombCount;

    return true;
}

unsigned int Settings::getBoardSizeX() const {
    return this->boardSizeX;
}

unsigned int Settings::getBoardSizeY() const {
    return this->boardSizeY;
}

unsigned int Settings::getBombCount() const {
    return this->bombCount;
}

std::string Settings::difficultyToString(const Difficulty& difficulty) {
    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END),  "Difficulty enum version mismatch");

    switch(difficulty) {
        case (Difficulty::EASY):   return "EASY";
        case (Difficulty::MEDIUM): return "MEDIUM";
        case (Difficulty::HARD):   return "HARD";
        case (Difficulty::CUSTOM): return "CUSTOM";
        default: assert(false);
    }

    return ""; // should not be reached
}

Difficulty Settings::stringToDifficulty(const std::string& string) {
    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END),  "Difficulty enum version mismatch");

    if      (string == "EASY")   return Difficulty::EASY;
    else if (string == "MEDIUM") return Difficulty::MEDIUM;
    else if (string == "HARD")   return Difficulty::HARD;
    else if (string == "CUSTOM") return Difficulty::CUSTOM;
    else assert(false);

    return Difficulty::CUSTOM; // should not be reached
}
