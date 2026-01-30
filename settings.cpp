#include "settings.h"

// TODO maybe introduce enum as return value instead of bool for better exception handling
bool Settings::setSettings(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount) {
    if (boardSizeX < 3 || boardSizeY < 3 || boardSizeX > 10 || boardSizeY > 10) return false; // minimum boardSize is 3x3, maximum boardSize is 10x10
    if (bombCount < 1 || bombCount >= boardSizeX * boardSizeY / 2) return false; // minimum bombCount is 1, maximum bombCount is boardSizeX * boardSizeY / 2

    this->boardSizeX = boardSizeX;
    this->boardSizeY = boardSizeY;
    this->bombCount = bombCount;

    return true;
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
