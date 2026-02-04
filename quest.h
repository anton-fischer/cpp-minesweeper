#pragma once

#include <QObject>

#define QUEST_TYPE_ENUM_GUARD 4
enum class QuestType {
    PLAY_GAMES = 0,
    WIN_GAMES = 1,
    PLACE_FLAGS = 2,
    UNCOVER_TILES = 3,

    _END = 4
};

class Quest : public QObject
{
    Q_OBJECT

public:
    explicit Quest(QObject* parent = nullptr);

    std::string generateObjectiveString() const;

    QuestType getType() const;

    unsigned int getGoal() const;
    unsigned int getReward() const;

    unsigned int getProgress() const;
    void setProgress(const unsigned int progress);

signals:
    void questCompleted();

private:
    const QuestType type;

    const unsigned int goal;
    const unsigned int reward;

    unsigned int progress;
};
