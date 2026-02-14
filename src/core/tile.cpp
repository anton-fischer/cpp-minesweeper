#include "core/tile.h"

Tile::Tile() {}

bool Tile::getIsBomb() const {
    return isBomb;
}

bool Tile::getIsFlag() const {
    return isFlag;
}

bool Tile::getIsCovered() const {
    return isCovered;
}

unsigned int Tile::getNumber() const {
    return number;
}

void Tile::setIsBomb(bool isBomb) {
    this->isBomb = isBomb;
}

void Tile::setIsFlag(bool isFlag) {
    this->isFlag = isFlag;
}

void Tile::setIsCovered(bool isCovered) {
    this->isCovered = isCovered;
}

void Tile::setNumber(unsigned int number) {
    this->number = number;
}

void Tile::incrementNumber() {
    this->number += 1;
}

std::unique_ptr<Tile> Tile::fromJson(const nlohmann::json& j) {
    std::unique_ptr<Tile> t = std::make_unique<Tile>();

    // general
    t->isBomb    = j.at("isBomb");
    t->isFlag    = j.at("isFlag");
    t->isCovered = j.at("isCovered");
    t->number    = j.at("number");

    return t;
}

nlohmann::json Tile::toJson() const {
    nlohmann::json j;

    // general
    j["isBomb"]    = isBomb;
    j["isFlag"]    = isFlag;
    j["isCovered"] = isCovered;
    j["number"]    = number;

    return j;
}

QDebug operator<<(QDebug dbg, const Tile& t) {
    return dbg << "isBomb[" << t.isBomb << "] isFlag[" << t.isFlag << "] isCovered[" << t.isCovered << "] number[" << t.number << "]";
}
