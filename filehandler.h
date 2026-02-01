#pragma once

#include <string>

class Board;
class Player;

class FileHandler
{
public:
    FileHandler();

    Board createBoardFromFile(std::string filepath);
    Player createPlayerFromFile(std::string filepath);
};
