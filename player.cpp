#include "player.h"

unsigned int Player::playerCount = 0;

Player::Player() : Player("Player " + std::to_string(playerCount + 1)) {}

Player::Player(const std::string& name) : name(name) {
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
