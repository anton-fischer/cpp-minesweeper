#include "core/quest.h"

#include <sstream>

#include <QRandomGenerator>
#include <QDebug>

Quest::Quest(QObject* parent) : QObject(parent) {
    generateQuest();
}

void Quest::generateQuest() {
    this->type =   static_cast<QuestType>  (QRandomGenerator::global()->bounded(1, static_cast<int>(QuestType::_END)));
    this->rarity = static_cast<QuestRarity>(QRandomGenerator::global()->bounded(1, static_cast<int>(QuestRarity::_END)));

    auto& questTypeData =   TYPE_CONVERSION_TABLE[static_cast<int>(type)];
    auto& questRarityData = RARITY_CONVERSION_TABLE[static_cast<int>(rarity)];

    const unsigned int generatedQuestGoal = QRandomGenerator::global()->bounded(questTypeData.minGoal, questTypeData.maxGoal + 1);

    this->goal = std::round(generatedQuestGoal * questRarityData.difficultyMultiplier);
    this->reward = std::round(goal * questTypeData.xpPerUnit * questRarityData.rewardMultiplier);

    this->progress = 0;
    this->completed = false;

    qDebug() << "Generated quest: " << *this;
}

QString Quest::generateObjectiveString() const {
    std::ostringstream returnString;

    static_assert(QUEST_TYPE_ENUM_GUARD == static_cast<int>(QuestType::_END), "QuestType enum version mismatch");
    switch(type) {
        case QuestType::PLAY_GAMES: returnString << "Play " << goal << " Games"; break;
        case QuestType::WIN_GAMES: returnString << "Win " << goal << " Games"; break;
        case QuestType::PLACE_FLAGS: returnString << "Place " << goal << " Flags"; break;
        case QuestType::UNCOVER_TILES: returnString << "Uncover " << goal << " Tiles"; break;

        default: assert(false);
    }

    return QString::fromStdString(returnString.str());
}

void Quest::regenerateQuest() {
    this->generateQuest();
}

QuestType Quest::getType() const {
    return this->type;
}

QuestRarity Quest::getRarity() const {
    return this->rarity;
}

unsigned int Quest::getGoal() const {
    return this->goal;
}

unsigned int Quest::getReward() const {
    return this->reward;
}

unsigned int Quest::getProgress() const {
    return this->progress;
}

void Quest::advanceProgress(const unsigned int newProgress) {
    if (completed) return;

    if (this->progress + newProgress >= goal) {
        this->progress = goal;
        completed = true;
        qDebug() << QString("Completed Quest: type[%1] progress[%2/%3]").arg(questTypeToString(type)).arg(progress).arg(goal);
    } else {
        this->progress += newProgress;
        qDebug() << QString("Advanced Quest: type[%1] progress[%2/%3]").arg(questTypeToString(type)).arg(progress).arg(goal);
        emit questUpdated(this);
    }
}

void Quest::setProgress(const unsigned int newProgress) {
    if (completed) return;

    if (newProgress >= goal) {
        this->progress = goal;
        completed = true;
        qDebug() << QString("Completed Quest: type[%1] progress[%2/%3]").arg(questTypeToString(type)).arg(progress).arg(goal);
        emit questCompleted(this);
    } else {
        this->progress = newProgress;
        qDebug() << QString("Advanced Quest: type[%1] progress[%2/%3]").arg(questTypeToString(type)).arg(progress).arg(goal);
        emit questUpdated(this);
    }
}

QString Quest::questTypeToString(const QuestType& questType) {
    static_assert(QUEST_TYPE_ENUM_GUARD == static_cast<int>(QuestType::_END), "QuestType enum version mismatch");

    switch(questType) {
        case (QuestType::PLAY_GAMES):    return "PLAY GAMES";
        case (QuestType::WIN_GAMES):     return "WIN GAMES";
        case (QuestType::PLACE_FLAGS):   return "PLACE FLAGS";
        case (QuestType::UNCOVER_TILES): return "UNCOVER TILES";
        default: assert(false);
    }

    return ""; // should not be reached
}

QuestType Quest::stringToQuestType(const QString& string) {
    static_assert(QUEST_TYPE_ENUM_GUARD == static_cast<int>(QuestType::_END), "QuestType enum version mismatch");

    if      (string == "PLAY GAMES")    return QuestType::PLAY_GAMES;
    else if (string == "WIN GAMES")     return QuestType::WIN_GAMES;
    else if (string == "PLACE FLAGS")   return QuestType::PLACE_FLAGS;
    else if (string == "UNCOVER TILES") return QuestType::UNCOVER_TILES;
    else assert(false);

    return QuestType::_END; // should not be reached
}

QString Quest::questRarityToString(const QuestRarity& questRarity) {
    static_assert(QUEST_RARITY_ENUM_GUARD == static_cast<int>(QuestRarity::_END), "QuestRarity enum version mismatch");

    switch(questRarity) {
    case (QuestRarity::COMMON):    return "COMMON";
    case (QuestRarity::RARE):      return "RARE";
    case (QuestRarity::EPIC):      return "EPIC";
    case (QuestRarity::LEGENDARY): return "LEGENDARY";
    default: assert(false);
    }

    return ""; // should not be reached
}

QuestRarity Quest::stringToQuestRarity(const QString& string) {
    static_assert(QUEST_RARITY_ENUM_GUARD == static_cast<int>(QuestRarity::_END),"QuestRarity enum version mismatch");

    if      (string == "COMMON")    return QuestRarity::COMMON;
    else if (string == "RARE")      return QuestRarity::RARE;
    else if (string == "EPIC")      return QuestRarity::EPIC;
    else if (string == "LEGENDARY") return QuestRarity::LEGENDARY;
    else assert(false);

    return QuestRarity::_END; // should not be reached
}

std::unique_ptr<Quest> Quest::fromJson(const nlohmann::json& j, QObject* parent) {
    std::unique_ptr<Quest> q = std::make_unique<Quest>();

    // general
    q->type     = stringToQuestType(QString::fromStdString(j.at("type")));
    q->rarity   = stringToQuestRarity(QString::fromStdString(j.at("rarity")));
    q->goal     = j.at("goal");
    q->reward   = j.at("reward");
    q->progress = j.at("progress");

    return q;
}

nlohmann::json Quest::toJson() const {
    nlohmann::json j;

    // general
    j["type"]     = questTypeToString(type).toStdString();
    j["rarity"]   = questRarityToString(rarity).toStdString();
    j["goal"]     = goal;
    j["reward"]   = reward;
    j["progress"] = progress;

    return j;
}

QDebug operator<<(QDebug dbg, const Quest& q) {
    return dbg << "type[" << Quest::questTypeToString(q.type) << "] rarity[" << Quest::questRarityToString(q.rarity) << "] goal[" << q.goal << "] reward[" << q.reward << "] progress[" << q.progress << "]";
}
