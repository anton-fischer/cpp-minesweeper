#include "core/settings.h"

#include <QDebug>
#include <assert.h>

#include <utils/filehandler.h>

Settings& Settings::instance() {
    static Settings instance;

    if (!instance.getIsInitialized()) {
        instance.setIsInitialized(true);

        FileHandler handler;
        bool success = handler.loadSettings();

        if (success) {
            qDebug() << "Successfully restored Settings from save file";
        } else {
            qDebug() << "Could not restore settings from save file, using default settings instead";
        }
    }

    return instance;
}

// TODO maybe introduce enum as return value instead of bool for better exception handling
bool Settings::setSettings(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount) {
    //if (boardSizeX < 3 || boardSizeY < 3 || boardSizeX > 10 || boardSizeY > 10) return false; // minimum boardSize is 3x3, maximum boardSize is 10x10
    //if (bombCount < 1 || bombCount >= boardSizeX * boardSizeY / 2) return false; // minimum bombCount is 1, maximum bombCount is boardSizeX * boardSizeY / 2

    this->difficulty = Difficulty::CUSTOM;
    this->boardSizeX = boardSizeX;
    this->boardSizeY = boardSizeY;
    this->bombCount  = bombCount;

    return true;
}

bool Settings::setSettings(const Difficulty difficulty) {
    auto& difficultyData = DIFFICULTY_DATA_TABLE[static_cast<int>(difficulty)];

    this->difficulty = difficulty;
    this->boardSizeX = difficultyData.boardSizeX;
    this->boardSizeY = difficultyData.boardSizeY;
    this->bombCount  = difficultyData.bombCount;

    return true;
}

bool Settings::getIsInitialized() const {
    return this->isInitialized;
}

void Settings::setIsInitialized(bool isInitialized) {
    this->isInitialized = isInitialized;
}

std::vector<Highscore>& Settings::getHighscores() {
    return this->highscores;
}

void Settings::addHighscore(Highscore& highscore) {
    this->highscores.push_back(highscore);
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

Difficulty Settings::getDifficulty() const {
    return this->difficulty;
}

std::unique_ptr<Player>& Settings::getCurrentPlayer() {
    //assert(nullptr != currentPlayer);
    return currentPlayer;
}

void Settings::setCurrentPlayer(std::unique_ptr<Player> player) {
    this->currentPlayer = std::move(player);
}

void Settings::fromJson(const nlohmann::json& j) {
    // general
    assert(j.at("saveVersion") == SETTINGS_SAVE_FILE_VERSION && "save file version missmatch detected while loading settings");

    const Difficulty difficulty = DifficultyUtil::stringToDifficulty(QString::fromStdString(j.at("general").at("difficulty")));
    const unsigned int boardSizeX = j.at("general").at("boardSizeX");
    const unsigned int boardSizeY = j.at("general").at("boardSizeY");
    const unsigned int bombCount  = j.at("general").at("bombCount");

    if (difficulty == Difficulty::CUSTOM) {
        Settings::instance().setSettings(boardSizeX, boardSizeY, bombCount);
    } else {
        Settings::instance().setSettings(difficulty);
    }

    // highscores
    if (j.contains("highscores") && j["highscores"].is_array()) {
        Settings::instance().getHighscores().clear();
        for (auto& jh : j["highscores"]) {
            auto highscore = Highscore::fromJson(jh);
            Settings::instance().addHighscore(*highscore);
        }
    }
}

nlohmann::json Settings::toJson() const {
    nlohmann::json j;

    // general
    j["saveVersion"] = SETTINGS_SAVE_FILE_VERSION;

    j["general"]["difficulty"]  = DifficultyUtil::difficultyToString(this->difficulty).toStdString();
    j["general"]["boardSizeX"]  = this->boardSizeX;
    j["general"]["boardSizeY"]  = this->boardSizeY;
    j["general"]["bombCount"]   = this->bombCount;

    // highscores
    for (auto& highscore : Settings::instance().getHighscores()) {
        auto jh = highscore.toJson();
        j["highscores"].push_back(std::move(jh));
    }

    return j;
}

QDebug operator<<(QDebug dbg, const Settings& s) {
    return dbg << "boardSizeX[" << s.boardSizeX << "] boardSizeY[" << s.boardSizeY << "] bombCount[" << s.bombCount << "] difficulty[" << DifficultyUtil::difficultyToString(s.difficulty) << "]";
}
