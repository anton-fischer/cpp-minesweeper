#include "core/difficulty.h"

#include <QString>

QString DifficultyUtil::difficultyToString(const Difficulty& difficulty) {
    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END), "Difficulty enum version mismatch");

    switch(difficulty) {
    case (Difficulty::EASY):   return "EASY";
    case (Difficulty::MEDIUM): return "MEDIUM";
    case (Difficulty::HARD):   return "HARD";
    case (Difficulty::CUSTOM): return "CUSTOM";
    default: assert(false);
    }

    return ""; // should not be reached
}

Difficulty DifficultyUtil::stringToDifficulty(const QString& string) {
    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END), "Difficulty enum version mismatch");

    if      (string == "EASY")   return Difficulty::EASY;
    else if (string == "MEDIUM") return Difficulty::MEDIUM;
    else if (string == "HARD")   return Difficulty::HARD;
    else if (string == "CUSTOM") return Difficulty::CUSTOM;
    else assert(false);

    return Difficulty::_END; // should not be reached
}

float DifficultyUtil::getDifficultyXpMultiplier(const Difficulty& difficulty) {
    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END), "Difficulty enum version mismatch");

    switch(difficulty) {
    case (Difficulty::EASY):   return 1;
    case (Difficulty::MEDIUM): return 1.5;
    case (Difficulty::HARD):   return 2;
    case (Difficulty::CUSTOM): return 1;
    default: assert(false);
    }

    return 0; // should not be reached
}
