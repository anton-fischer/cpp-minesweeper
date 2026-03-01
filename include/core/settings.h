#pragma once

#include <memory>

#include "core/difficulty.h"
#include "core/player.h"
#include "core/highscore.h"

#define SETTINGS_SAVE_FILE_VERSION 1

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
