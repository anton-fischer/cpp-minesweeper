#include "player.h"

unsigned int Player::playerCount = 0;

Player::Player(QObject* parent) : Player("Player " + std::to_string(playerCount + 1), parent) {}

Player::Player(const std::string& name, QObject* parent) : name(name), QObject(parent) {
    ++playerCount;
}

std::string Player::getName() const {
    return this->name;
}

unsigned int Player::getScore() const {
    return this->score;
}

unsigned int Player::getPlayerCount() {
    return playerCount;
}
