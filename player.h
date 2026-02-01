#pragma once

#include <string>

class Player
{
public:
    Player();
    Player(const std::string& name);

    std::string getName() const;
    unsigned int getScore() const;

    static unsigned int getPlayerCount();

private:
    const std::string name;

    unsigned int score = 0;

    static unsigned int playerCount;
};
