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

    void saveBoardAsFile(Board* board, std::string filepath = "./boardSave.json");
    void savePlayerAsFile(Player* player, std::string filepath = "./playerSave.json");

    std::unique_ptr<Board>  createBoardFromFile(std::string filepath = "./boardSave.json");
    std::unique_ptr<Player> createPlayerFromFile(std::string filepath = "./playerSave.json");
};
