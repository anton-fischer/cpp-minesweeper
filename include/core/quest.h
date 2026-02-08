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
    ~Quest() = default;

    void regenerateQuest();

    std::string generateObjectiveString() const;

    QuestType getType() const;

    unsigned int getGoal() const;
    unsigned int getReward() const;

    unsigned int getProgress() const;
    void advanceProgress(const unsigned int newProgress);
    void setProgress(const unsigned int newProgress);

    static std::string questTypeToString(const QuestType& questType);
    static QuestType   stringToQuestType(const std::string& string);

signals:
    void questCompleted(Quest* quest);

private:
    void generateQuest();

    QuestType type;

    unsigned int goal;
    unsigned int reward;

    unsigned int progress;
};
