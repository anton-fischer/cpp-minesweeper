#pragma once

#include <array>

class QString;

#define DIFFICULTY_ENUM_GUARD 4
enum class Difficulty {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2,
    CUSTOM = 3,

    _END = 4
};

struct DifficultyData {
    unsigned int boardSizeX;
    unsigned int boardSizeY;
    unsigned int bombCount;
};

static const std::array<DifficultyData, static_cast<int>(Difficulty::_END)> DIFFICULTY_DATA_TABLE = {{
    {9,  9,  10},   // EASY
    {16, 16, 40},   // MEDIUM
    {30, 16, 99},   // HARD
    {0,  0,   0}    // CUSTOM
}};

namespace DifficultyUtil {
    QString    difficultyToString(const Difficulty& difficulty);
    Difficulty stringToDifficulty(const QString& string);
    float      getDifficultyXpMultiplier(const Difficulty& difficulty);
}
