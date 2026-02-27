#pragma once

#include <string>

#include "json.hpp"

#define SAVE_FILE_VERSION 1

using json = nlohmann::json;

class Board;
class Player;

class FileHandler
{
public:
    FileHandler();
    ~FileHandler() = default;

    void saveBoardAsFile(Board* board, std::string filepath);
    void savePlayerAsFile(Player* player, std::string filepath);

    std::unique_ptr<Board>  createBoardFromFile(std::string filepath);
    std::unique_ptr<Player> createPlayerFromFile(std::string filepath);
};
