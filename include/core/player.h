#pragma once

#include <QObject>
#include <string>
#include <memory>

#include "quest.h"

class Player : public QObject
{
    Q_OBJECT

public:
    Player(QObject* parent = nullptr);
    Player(const std::string& name, QObject* parent = nullptr);
    ~Player() = default;

    std::string getName() const;

    unsigned int getLevel() const;
    unsigned int getMaxXp() const;
    unsigned int getCurrentXp() const;

    std::vector<std::unique_ptr<Quest>>& getQuests();

    static unsigned int getPlayerCount();

    // stats
    unsigned int getAmountEasyGamesWon() const;
    unsigned int getAmountMediumGamesWon() const;
    unsigned int getAmountHardGamesWon() const;
    unsigned int getAmountCustomGamesWon() const;
    unsigned int getAmountBombsHit() const;
    unsigned int getAmountFlagsPlaced() const;
    unsigned int getAmountTilesUncovered() const;

    void incrementAmountEasyGamesWon(const unsigned int amount = 1);
    void incrementAmountMediumGamesWon(const unsigned int amount = 1);
    void incrementAmountHardGamesWon(const unsigned int amount = 1);
    void incrementAmountCustomGamesWon(const unsigned int amount = 1);
    void incrementAmountBombsHit(const unsigned int amount = 1);
    void incrementAmountFlagsPlaced(const unsigned int amount = 1);
    void incrementAmountTilesUncovered(const unsigned int amount = 1);

    void decrementAmountFlagsPlaced(const unsigned int amount = 1);

private:
    const std::string name;

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
    unsigned int amountBombsHit = 0;
    unsigned int amountFlagsPlaced = 0;
    unsigned int amountTilesUncovered = 0;
};
