#include "core/quest.h"

#include <sstream>

#include <QRandomGenerator>
#include <QDebug>

Quest::Quest(QObject* parent) : QObject(parent) {
    generateQuest();
}

void Quest::generateQuest() {
    static_assert(QUEST_TYPE_ENUM_GUARD == static_cast<int>(QuestType::_END),  "QuestType enum version mismatch");

    const unsigned int type = QRandomGenerator::global()->bounded(1, static_cast<int>(QuestType::_END));

    switch(static_cast<QuestType>(type)) {
        case QuestType::PLAY_GAMES: {
            this->type = QuestType::PLAY_GAMES;
            this->goal = QRandomGenerator::global()->bounded(1, 51);
            this->reward = goal * 10;
            break;
        }
        case QuestType::WIN_GAMES: {
            this->type = QuestType::WIN_GAMES;
            this->goal = QRandomGenerator::global()->bounded(1, 21);
            this->reward = goal * 100;
            break;
        }
        case QuestType::PLACE_FLAGS: {
            this->type = QuestType::PLACE_FLAGS;
            this->goal = QRandomGenerator::global()->bounded(1, 101);
            this->reward = goal * 0.5;
            break;
        }
        case QuestType::UNCOVER_TILES: {
            this->type = QuestType::UNCOVER_TILES;
            this->goal = QRandomGenerator::global()->bounded(1, 501);
            this->reward = goal * 0.25;
            break;
        }

        default: assert(false);
    }

    this->progress = 0;

    qDebug() << QString("Generated Quest: type[%1] goal[%2] reward[%3]").arg(questTypeToString(this->type)).arg(goal).arg(reward);
}

std::string Quest::generateObjectiveString() const {
    std::ostringstream returnString;

    static_assert(QUEST_TYPE_ENUM_GUARD == static_cast<int>(QuestType::_END),  "QuestType enum version mismatch");
    switch(type) {
        case QuestType::PLAY_GAMES: returnString << "Play " << goal << " Games"; break;
        case QuestType::WIN_GAMES: returnString << "Win " << goal << " Games"; break;
        case QuestType::PLACE_FLAGS: returnString << "Place " << goal << " Flags"; break;
        case QuestType::UNCOVER_TILES: returnString << "Uncover " << goal << " Tiles"; break;

        default: assert(false);
    }

    returnString << " [" << std::to_string(reward) << "XP]";

    return returnString.str();
}

void Quest::regenerateQuest() {
    this->generateQuest();
}

QuestType Quest::getType() const {
    return this->type;
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
    if (this->progress + newProgress >= goal) {
        this->progress = goal;
        emit questCompleted(this);
        return;
    }

    this->progress += newProgress;

    qDebug() << QString("Advanced Quest: type[%1] progress[%2/%3]").arg(questTypeToString(type)).arg(progress).arg(goal);
}

void Quest::setProgress(const unsigned int newProgress) {
    if (newProgress >= goal) {
        this->progress = goal;
        emit questCompleted(this);
        return;
    }

    this->progress = newProgress;
    qDebug() << QString("Advanced Quest: type[%1] progress[%2/%3]").arg(questTypeToString(type)).arg(progress).arg(goal);
}

std::string Quest::questTypeToString(const QuestType& questType) {
    static_assert(QUEST_TYPE_ENUM_GUARD == static_cast<int>(QuestType::_END),  "QuestType enum version mismatch");

    switch(questType) {
        case (QuestType::PLAY_GAMES):    return "PLAY GAMES";
        case (QuestType::WIN_GAMES):     return "WIN GAMES";
        case (QuestType::PLACE_FLAGS):   return "PLACE FLAGS";
        case (QuestType::UNCOVER_TILES): return "UNCOVER TILES";
        default: assert(false);
    }

    return ""; // should not be reached
}

QuestType Quest::stringToQuestType(const std::string& string) {
    static_assert(QUEST_TYPE_ENUM_GUARD == static_cast<int>(QuestType::_END),  "QuestType enum version mismatch");

    if      (string == "PLAY GAMES")    return QuestType::PLAY_GAMES;
    else if (string == "WIN GAMES")     return QuestType::WIN_GAMES;
    else if (string == "PLACE FLAGS")   return QuestType::PLACE_FLAGS;
    else if (string == "UNCOVER TILES") return QuestType::UNCOVER_TILES;
    else assert(false);

    return QuestType::_END; // should not be reached
}
