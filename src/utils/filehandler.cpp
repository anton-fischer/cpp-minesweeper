#include "utils/filehandler.h"

#include "core/board.h"
#include "core/player.h"

FileHandler::FileHandler() {}

Player FileHandler::createPlayerFromFile(std::string filepath) {
    return Player("test");
}

Board FileHandler::createBoardFromFile(std::string filepath) {
    return Board();
}
