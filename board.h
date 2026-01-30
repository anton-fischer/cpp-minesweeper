#pragma once

#include <vector>
#include "Tile.h"

class Board
{
public:
    Board(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount); // generate board based on values
    Board(); // generate board based on current settings

    bool placeFlag(const unsigned int x, const unsigned int y);

private:
    unsigned int boardSizeX;
    unsigned int boardSizeY;

    unsigned int bombCount;

    std::vector<std::vector<Tile>> board;

    void initBoard();
};
