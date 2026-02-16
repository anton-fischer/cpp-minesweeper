#pragma once

#include <QObject>
#include "utils/json.hpp"

#define QUEST_TYPE_ENUM_GUARD 4
enum class QuestType {
    PLAY_GAMES = 0,
    WIN_GAMES = 1,
    PLACE_FLAGS = 2,
    UNCOVER_TILES = 3,

    _END = 4
};

#define QUEST_RARITY_ENUM_GUARD 4
enum class QuestRarity {
    COMMON = 0,
    RARE = 1,
    EPIC = 2,
    LEGENDARY = 3,

    _END = 4
};

struct QuestTypeData {
    unsigned int minGoal;
    unsigned int maxGoal;
    float xpPerUnit;
};

struct QuestRarityData {
    float difficultyMultiplier;
    float rewardMultiplier;
};

static const std::array<QuestTypeData, static_cast<int>(QuestType::_END)> TYPE_CONVERSION_TABLE = {{
    {5, 25, 10.f},    // PLAY_GAMES
    {2, 10, 40.f},    // WIN_GAMES
    {20, 80, 4.f},    // PLACE_FLAGS
    {80, 400, 1.2f}   // UNCOVER_TILES
}};

static const std::array<QuestRarityData, static_cast<int>(QuestRarity::_END)> RARITY_CONVERSION_TABLE = {{
    {0.8f, 0.8f},   // COMMON
    {1.0f, 1.0f},   // RARE
    {1.4f, 1.6f},   // EPIC
    {2.0f, 2.8f}    // LEGENDARY
}};

class Quest : public QObject
{
    Q_OBJECT

public:
    explicit Quest(QObject* parent = nullptr);
    ~Quest() = default;

    void regenerateQuest();

    QString generateObjectiveString() const;

    QuestType   getType() const;
    QuestRarity getRarity() const;

    unsigned int getGoal() const;
    unsigned int getReward() const;

    unsigned int getProgress() const;
    void advanceProgress(const unsigned int newProgress);
    void setProgress(const unsigned int newProgress);

    static QString   questTypeToString(const QuestType& questType);
    static QuestType stringToQuestType(const QString& string);

    static QString     questRarityToString(const QuestRarity& questRarity);
    static QuestRarity stringToQuestRarity(const QString& string);

    // persistor
    static std::unique_ptr<Quest> fromJson(const nlohmann::json& j, QObject* parent = nullptr);
    nlohmann::json toJson() const;

    // toString
    friend QDebug operator<<(QDebug dbg, const Quest& q);

signals:
    void questCompleted(Quest* quest);

private:
    void generateQuest();

    QuestType   type;
    QuestRarity rarity;

    unsigned int goal;
    unsigned int reward;
    unsigned int progress;

    bool completed = false;
};
