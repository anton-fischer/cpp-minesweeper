#include "core/player.h"

#include <QFile>
#include <QDebug>

#include "core/quest.h"

unsigned int Player::playerCount = 0;

Player::Player(QObject* parent) : Player(QString("Player %1").arg(playerCount + 1), parent) {}

Player::Player(const QString& name, QObject* parent) : name(name), QObject(parent) {
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

QString Player::getName() const {
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

    while (currentXp >= maxXp) {
        currentXp -= maxXp;  // take rest xp
        level++;             // level up
        maxXp = maxXp * 1.5; // increase needed xp for level up

        emit playerLevelUp();
    }

    emit playerXpChange();
}

void Player::decrementXp(const unsigned int amount) {
    if (amount >= this->currentXp) this->currentXp = 0;
    else this->currentXp -= amount;

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

    // revert quests if available
    for (auto& quest : this->quests) {
        if (quest->getType() == QuestType::PLACE_FLAGS) {
            unsigned int currentProgress = quest->getProgress();
            quest->setProgress(currentProgress - amount);
        }
    }
}

std::unique_ptr<Player> Player::fromJson(const nlohmann::json& j, QObject* parent) {
    std::unique_ptr<Player> p = std::make_unique<Player>();

    // general
    assert(j.at("saveVersion") == PLAYER_SAVE_FILE_VERSION && "save file version missmatch detected while loading player");
    p->name = QString::fromStdString(j.at("name"));

    // progress
    p->level     = j.at("progress").at("level");
    p->maxXp     = j.at("progress").at("maxXp");
    p->currentXp = j.at("progress").at("currentXp");

    // stats
    p->amountEasyGamesWon   = j.at("stats").at("amountEasyGamesWon");
    p->amountMediumGamesWon = j.at("stats").at("amountMediumGamesWon");
    p->amountHardGamesWon   = j.at("stats").at("amountHardGamesWon");
    p->amountCustomGamesWon = j.at("stats").at("amountCustomGamesWon");
    p->amountGamesPlayed    = j.at("stats").at("amountGamesPlayed");
    p->amountBombsHit       = j.at("stats").at("amountBombsHit");
    p->amountFlagsPlaced    = j.at("stats").at("amountFlagsPlaced");
    p->amountTilesUncovered = j.at("stats").at("amountTilesUncovered");

    // quests
    p->quests.clear();
    for (auto& jq : j.at("quests")) {
        auto quest = Quest::fromJson(jq);
        p->quests.push_back(std::move(quest));
    }

    return p;
}

nlohmann::json Player::toJson() const {
    nlohmann::json j;
    // general
    j["saveVersion"] = PLAYER_SAVE_FILE_VERSION;
    j["name"]        = this->name.toStdString();

    // progress
    j["progress"]["level"]     = this->level;
    j["progress"]["maxXp"]     = this->maxXp;
    j["progress"]["currentXp"] = this->currentXp;

    // stats
    j["stats"]["amountEasyGamesWon"]   = this->amountEasyGamesWon;
    j["stats"]["amountMediumGamesWon"] = this->amountMediumGamesWon;
    j["stats"]["amountHardGamesWon"]   = this->amountHardGamesWon;
    j["stats"]["amountCustomGamesWon"] = this->amountCustomGamesWon;
    j["stats"]["amountGamesPlayed"]    = this->amountGamesPlayed;
    j["stats"]["amountBombsHit"]       = this->amountBombsHit;
    j["stats"]["amountFlagsPlaced"]    = this->amountFlagsPlaced;
    j["stats"]["amountTilesUncovered"] = this->amountTilesUncovered;

    // quests
    for (auto& quest : this->quests) {
        auto jq = quest->toJson();
        j["quests"].push_back(std::move(jq));
    }

    return j;
}

QDebug operator<<(QDebug dbg, const Player& p) {
    return dbg << "level[" << p.level << "] maxXp[" << p.maxXp << "] currentXp[" << p.currentXp << "]";
}
