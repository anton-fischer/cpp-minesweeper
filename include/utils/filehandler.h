#pragma once

#include <string>

#include "json.hpp"

using json = nlohmann::json;

class Board;
class Player;

class FileHandler
{
public:
    FileHandler();
    ~FileHandler() = default;

    bool saveSettings(std::string filepath = "./settings.json");
    bool loadSettings(std::string filepath = "./settings.json");

    bool saveBoardAsFile(Board* board, std::string filepath);
    bool savePlayerAsFile(Player* player, std::string filepath);

    std::unique_ptr<Board>  createBoardFromFile(std::string filepath);
    std::unique_ptr<Player> createPlayerFromFile(std::string filepath);
};
