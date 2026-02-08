#pragma once

#include <string>
#include <memory>

#include "core/player.h"

#define DIFFICULTY_ENUM_GUARD 4
enum class Difficulty {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2,
    CUSTOM = 3,

    _END = 4
};

class Settings
{
public:
    // singleton
    static Settings& instance() {
        static Settings instance;
        return instance;
    }

    // updates settings and returns true for valid input, false for invalid input
    // nothing is changed when false returned
    bool setSettings(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount);

    std::unique_ptr<Player>& getCurrentPlayer();
    void setCurrentPlayer(std::unique_ptr<Player>& player);

    Difficulty getDifficulty() const;
    void setDifficulty(Difficulty difficulty);

    unsigned int getBoardSizeX() const;
    unsigned int getBoardSizeY() const;
    unsigned int getBombCount() const;

    static std::string difficultyToString(const Difficulty& difficulty);
    static Difficulty  stringToDifficulty(const std::string& string);
    static float       getDifficultyXpMultiplier(const Difficulty& difficulty);

private:
    // private constructor
    Settings() = default;
    ~Settings() = default;

    // forbid copying
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    std::unique_ptr<Player> currentPlayer = nullptr;

    Difficulty difficulty = Difficulty::CUSTOM;

    unsigned int boardSizeX = 10;
    unsigned int boardSizeY = 10;
    unsigned int bombCount = 10;
};
