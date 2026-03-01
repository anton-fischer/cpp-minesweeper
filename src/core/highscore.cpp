#include "core/highscore.h"

#include "core/player.h"
#include "core/board.h"

Highscore::Highscore(Player* player, Board* board, unsigned int score) {
    this->name = player->getName();

    this->difficulty = board->getDifficulty();
    this->boardSizeX = board->getBoardSizeX();
    this->boardSizeY = board->getBoardSizeY();
    this->bombCount  = board->getBombCount();

    this->score      = score;
    this->startSeed  = board->getStartSeed();
}

QString Highscore::getName() const {
    return this->name;
}

unsigned int Highscore::getScore() const {
    return this->score;
}

Difficulty Highscore::getDifficulty() const {
    return this->difficulty;
}

unsigned int Highscore::getBoardSizeX() const {
    return this->boardSizeX;
}

unsigned int Highscore::getBoardSizeY() const {
    return this->boardSizeY;
}

unsigned int Highscore::getBombCount() const {
    return this->bombCount;
}

unsigned int Highscore::getStartSeed() const {
    return this->startSeed;
}

std::unique_ptr<Highscore> Highscore::fromJson(const nlohmann::json& j) {
    std::unique_ptr<Highscore> h = std::make_unique<Highscore>();

    // general
    h->name       = QString::fromStdString(j.at("name"));
    h->score      = j.at("score");

    h->difficulty = DifficultyUtil::stringToDifficulty(QString::fromStdString(j.at("difficulty")));
    h->boardSizeX = j.at("boardSizeX");
    h->boardSizeY = j.at("boardSizeY");
    h->bombCount  = j.at("bombCount");

    h->startSeed  = j.at("startSeed");

    return h;
}

nlohmann::json Highscore::toJson() const {
    nlohmann::json h;

    // general
    h["name"]       = name.toStdString();
    h["score"]      = score;

    h["difficulty"] = DifficultyUtil::difficultyToString(difficulty).toStdString();
    h["boardSizeX"] = boardSizeX;
    h["boardSizeY"] = boardSizeY;
    h["bombCount"]  = bombCount;

    h["startSeed"]  = startSeed;

    return h;
}

QDebug operator<<(QDebug dbg, const Highscore& t) {
    return dbg << "name[" << t.name << "] boardSizeX[" << t.boardSizeX << "] boardSizeY[" << t.boardSizeY << "] bombCount[" << t.bombCount << "] score[" << t.score << "] startSeed[" << t.startSeed << "]";
}
