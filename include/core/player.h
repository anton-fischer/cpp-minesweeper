#pragma once

#include <QObject>
#include <string>
#include <memory>

#include "quest.h"
#include "utils/json.hpp"

#define SAVE_FILE_VERSION 1

class Player : public QObject
{
    Q_OBJECT

public:
    Player(QObject* parent = nullptr);
    Player(const std::string& name, QObject* parent = nullptr);
    ~Player() = default;

    std::string getName() const;
    std::vector<std::unique_ptr<Quest>>& getQuests();

    static unsigned int getPlayerCount();

    // xp
    void incrementXp(const unsigned int amount = 1);
    void decrementXp(const unsigned int amount = 1); // doesn't revert level ups, avoid to use

    unsigned int getLevel() const;
    unsigned int getMaxXp() const;
    unsigned int getCurrentXp() const;

    // stats
    unsigned int getAmountEasyGamesWon() const;
    unsigned int getAmountMediumGamesWon() const;
    unsigned int getAmountHardGamesWon() const;
    unsigned int getAmountCustomGamesWon() const;
    unsigned int getAmountGamesPlayed() const;
    unsigned int getAmountBombsHit() const;
    unsigned int getAmountFlagsPlaced() const;
    unsigned int getAmountTilesUncovered() const;

    void incrementAmountEasyGamesWon(const unsigned int amount = 1);
    void incrementAmountMediumGamesWon(const unsigned int amount = 1);
    void incrementAmountHardGamesWon(const unsigned int amount = 1);
    void incrementAmountCustomGamesWon(const unsigned int amount = 1);
    void incrementAmountGamesPlayed(const unsigned int amount = 1);
    void incrementAmountBombsHit(const unsigned int amount = 1);
    void incrementAmountFlagsPlaced(const unsigned int amount = 1);
    void incrementAmountTilesUncovered(const unsigned int amount = 1);

    void decrementAmountFlagsPlaced(const unsigned int amount = 1);

    // persistor
    static std::unique_ptr<Player> fromJson(const nlohmann::json& j, QObject* parent = nullptr);
    nlohmann::json toJson() const;

    // toString
    friend QDebug operator<<(QDebug dbg, const Player& p);

signals:
    void playerLevelUp();
    void playerXpChange();

private slots:
    void questCompleted(Quest* quest);

private:
    std::string name;

    unsigned int level = 0;
    unsigned int maxXp = 100;
    unsigned int currentXp = 0;

    std::vector<std::unique_ptr<Quest>> quests;

    static unsigned int playerCount;

    // stats
    unsigned int amountEasyGamesWon = 0;
    unsigned int amountMediumGamesWon = 0;
    unsigned int amountHardGamesWon = 0;
    unsigned int amountCustomGamesWon = 0;
    unsigned int amountGamesPlayed = 0;
    unsigned int amountBombsHit = 0;
    unsigned int amountFlagsPlaced = 0;
    unsigned int amountTilesUncovered = 0;
};
