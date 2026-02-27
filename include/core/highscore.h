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

    // persistor
    static std::unique_ptr<Highscore> fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;

    // toString
    friend QDebug operator<<(QDebug dbg, const Highscore& h);

private:
    QString name = "";

    unsigned int score = 0;

    unsigned int boardSizeX = 0;
    unsigned int boardSizeY = 0;
    unsigned int bombCount = 0;

    unsigned int startSeed = 0;
};
