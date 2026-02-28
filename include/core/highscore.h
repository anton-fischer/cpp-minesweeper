#pragma once

#include <QString>

#include "utils/json.hpp"

class Player;
class Board;

class Highscore
{
public:
    Highscore() = default;
    Highscore(Player* player, Board* board, unsigned int score);
    ~Highscore() = default;

    QString getName() const;
    unsigned int getScore() const;
    unsigned int getBoardSizeX() const;
    unsigned int getBoardSizeY() const;
    unsigned int getBombCount() const;
    unsigned int getStartSeed() const;

    // persistor
    static std::unique_ptr<Highscore> fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;

    // toString
    friend QDebug operator<<(QDebug dbg, const Highscore& h);

private:
    QString name = "Uninitialized";

    unsigned int score = 0;

    unsigned int boardSizeX = 0;
    unsigned int boardSizeY = 0;
    unsigned int bombCount = 0;

    unsigned int startSeed = 0;
};
