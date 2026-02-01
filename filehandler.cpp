#include "filehandler.h"

#include "board.h"
#include "player.h"

FileHandler::FileHandler() {}

Player FileHandler::createPlayerFromFile(std::string filepath) {
    return Player("test");
}

Board FileHandler::createBoardFromFile(std::string filepath) {
    return Board();
}
