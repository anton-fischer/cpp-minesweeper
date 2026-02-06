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
