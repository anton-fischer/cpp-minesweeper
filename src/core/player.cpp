#include "core/player.h"

#include "core/quest.h"

unsigned int Player::playerCount = 0;

Player::Player(QObject* parent) : Player("Player " + std::to_string(playerCount + 1), parent) {}

Player::Player(const std::string& name, QObject* parent) : name(name), QObject(parent) {
    ++playerCount;

    // generate three quests
    quests.push_back(std::make_unique<Quest>());
    quests.push_back(std::make_unique<Quest>());
    quests.push_back(std::make_unique<Quest>());

    for (auto& quest : quests) {
        connect(quest.get(), &Quest::questCompleted, this, &Player::questCompleted);
    }
}

void Player::questCompleted(Quest* quest) {
    incrementXp(quest->getGoal());
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

unsigned int Player::getAmountGamesPlayed() const
{
    return this->amountGamesPlayed;
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

void Player::incrementXp(const unsigned int amount) {
    this->currentXp += amount;

    if (currentXp >= maxXp) {
        currentXp -= maxXp;  // take rest xp
        level++;             // level up
        maxXp = maxXp * 1.5; // increase needed xp for level up

        emit playerLevelUp();
    }

    emit playerXpChange();
}

void Player::incrementAmountEasyGamesWon(const unsigned int amount) {
    this->amountEasyGamesWon += amount;

    // advance quest if available
    for (auto& quest : this->quests) {
        if (quest->getType() == QuestType::WIN_GAMES) {
            quest->advanceProgress(amount);
        }
    }
}

void Player::incrementAmountMediumGamesWon(const unsigned int amount) {
    this->amountMediumGamesWon += amount;

    // advance quest if available
    for (auto& quest : this->quests) {
        if (quest->getType() == QuestType::WIN_GAMES) {
            quest->advanceProgress(amount);
        }
    }
}

void Player::incrementAmountHardGamesWon(const unsigned int amount) {
    this->amountHardGamesWon += amount;

    // advance quest if available
    for (auto& quest : this->quests) {
        if (quest->getType() == QuestType::WIN_GAMES) {
            quest->advanceProgress(amount);
        }
    }
}

void Player::incrementAmountCustomGamesWon(const unsigned int amount) {
    this->amountCustomGamesWon += amount;

    // advance quest if available
    for (auto& quest : this->quests) {
        if (quest->getType() == QuestType::WIN_GAMES) {
            quest->advanceProgress(amount);
        }
    }
}

void Player::incrementAmountGamesPlayed(const unsigned int amount) {
    this->amountGamesPlayed += amount;

    // advance quest if available
    for (auto& quest : this->quests) {
        if (quest->getType() == QuestType::PLAY_GAMES) {
            quest->advanceProgress(amount);
        }
    }
}

void Player::incrementAmountBombsHit(const unsigned int amount) {
    this->amountBombsHit += amount;

    // no quest type for this stat available
}

void Player::incrementAmountFlagsPlaced(const unsigned int amount) {
    this->amountFlagsPlaced += amount;

    // advance quest if available
    for (auto& quest : this->quests) {
        if (quest->getType() == QuestType::PLACE_FLAGS) {
            quest->advanceProgress(amount);
        }
    }
}

void Player::incrementAmountTilesUncovered(const unsigned int amount) {
    this->amountTilesUncovered += amount;

    // advance quest if available
    for (auto& quest : this->quests) {
        if (quest->getType() == QuestType::UNCOVER_TILES) {
            quest->advanceProgress(amount);
        }
    }
}

void Player::decrementAmountFlagsPlaced(const unsigned int amount) {
    assert(this->amountFlagsPlaced - amount >= 0);
    this->amountFlagsPlaced -= amount;

    // TODO implement quest logic for this case
}

