#include "core/highscore.h"

#include "core/player.h"
#include "core/board.h"

Highscore::Highscore(Player* player, Board* board, unsigned int score) {
    this->name = player->getName();

    this->boardSizeX = board->getBoardSizeX();
    this->boardSizeY = board->getBoardSizeY();
    this->bombCount  = board->getBombCount();

    this->score = score;
}

std::unique_ptr<Highscore> Highscore::fromJson(const nlohmann::json& j) {
    std::unique_ptr<Highscore> h = std::make_unique<Highscore>();

    // general
    h->name       = QString::fromStdString(j.at("name"));
    h->boardSizeX = j.at("boardSizeX");
    h->boardSizeY = j.at("boardSizeY");
    h->bombCount  = j.at("bombCount");
    h->score      = j.at("score");

    return h;
}

nlohmann::json Highscore::toJson() const {
    nlohmann::json h;

    // general
    h["name"]       = name.toStdString();
    h["boardSizeX"] = boardSizeX;
    h["boardSizeY"] = boardSizeY;
    h["bombCount"]  = bombCount;
    h["score"]      = score;

    return h;
}

QDebug operator<<(QDebug dbg, const Highscore& t) {
    return dbg << "name[" << t.name << "] boardSizeX[" << t.boardSizeX << "] boardSizeY[" << t.boardSizeY << "] bombCount[" << t.bombCount << "] score[" << t.score << "]";
}
