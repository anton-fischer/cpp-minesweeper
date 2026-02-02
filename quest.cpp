#include "quest.h"

#include <sstream>

Quest::Quest(QObject* parent) : type(QuestType::WIN_GAMES), goal(100), reward(100), QObject(parent) {

}

std::string Quest::generateObjectiveString() const {
    static_assert(QUEST_TYPE_ENUM_VERSION == 1,  "QuestType enum version mismatch");

    std::ostringstream returnString;

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

void Quest::setProgress(const unsigned int progress) {
    assert(progress <= goal);
    this->progress = progress;
}
