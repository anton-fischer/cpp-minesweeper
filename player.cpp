#include "player.h"

#include "quest.h"

unsigned int Player::playerCount = 0;

Player::Player(QObject* parent) : Player("Player " + std::to_string(playerCount + 1), parent) {}

Player::Player(const std::string& name, QObject* parent) : name(name), QObject(parent) {
    ++playerCount;

    // generate three quests
    quests.push_back(std::make_unique<Quest>());
    quests.push_back(std::make_unique<Quest>());
    quests.push_back(std::make_unique<Quest>());
}

std::string Player::getName() const {
    return this->name;
}

unsigned int Player::getLevel() const {
    return this->level;
}

unsigned int Player::getMaxXp() const {
    return this->maxXp;
}

unsigned int Player::getCurrentXp() const {
    return this->currentXp;
}

std::vector<std::unique_ptr<Quest>>& Player::getQuests() {
    return this->quests;
}

unsigned int Player::getAmountEasyGamesWon() const
{
    return this->amountEasyGamesWon;
}

unsigned int Player::getAmountMediumGamesWon() const
{
    return this->amountMediumGamesWon;
}

unsigned int Player::getAmountHardGamesWon() const
{
    return this->amountHardGamesWon;
}

unsigned int Player::getAmountCustomGamesWon() const
{
    return this->amountCustomGamesWon;
}

unsigned int Player::getAmountBombsHit() const
{
    return this->amountBombsHit;
}

unsigned int Player::getAmountFlagsPlaced() const
{
    return this->amountFlagsPlaced;
}

unsigned int Player::getAmountTilesUncovered() const
{
    return this->amountTilesUncovered;
}

unsigned int Player::getPlayerCount() {
    return playerCount;
}
