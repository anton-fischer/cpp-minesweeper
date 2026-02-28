#pragma once

#include <memory>

#include "core/player.h"
#include "core/highscore.h"

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

class Settings
{
public:
    // singleton
    static Settings& instance();

    // updates settings and returns true for valid input, false for invalid input
    // nothing is changed when false returned
    bool setSettings(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount);
    bool setSettings(const Difficulty difficulty);

    bool getIsInitialized() const;
    void setIsInitialized(bool isInitialized);

    std::unique_ptr<Player>& getCurrentPlayer();
    void setCurrentPlayer(std::unique_ptr<Player> player);

    std::vector<Highscore>& getHighscores();
    void addHighscore(Highscore& highscore);

    Difficulty getDifficulty() const;

    unsigned int getBoardSizeX() const;
    unsigned int getBoardSizeY() const;
    unsigned int getBombCount() const;

    static QString    difficultyToString(const Difficulty& difficulty);
    static Difficulty stringToDifficulty(const QString& string);
    static float      getDifficultyXpMultiplier(const Difficulty& difficulty);

    // persistor
    static void fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;

    // toString
    friend QDebug operator<<(QDebug dbg, const Settings& s);

private:
    // private constructor
    Settings() = default;
    ~Settings() = default;

    // forbid copying
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    // if false it will try to restore settings from savefile on usage
    bool isInitialized = false;

    std::vector<Highscore> highscores;

    std::unique_ptr<Player> currentPlayer = nullptr;

    Difficulty difficulty = Difficulty::EASY;

    unsigned int boardSizeX = DIFFICULTY_DATA_TABLE[static_cast<int>(difficulty)].boardSizeX;
    unsigned int boardSizeY = DIFFICULTY_DATA_TABLE[static_cast<int>(difficulty)].boardSizeY;
    unsigned int bombCount =  DIFFICULTY_DATA_TABLE[static_cast<int>(difficulty)].bombCount;
};
