#pragma once

#include <QObject>
#include <string>

class Player : public QObject
{
    Q_OBJECT

public:
    Player(QObject* parent = nullptr);
    Player(const std::string& name, QObject* parent = nullptr);

    std::string getName() const;
    unsigned int getScore() const;

    static unsigned int getPlayerCount();

private:
    const std::string name;

    unsigned int score = 0;

    static unsigned int playerCount;
};
