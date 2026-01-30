#pragma once

enum class TileType {
    EMPTY,
    BOMB,
    FLAG
};

class Tile
{
public:
    Tile();

private:
    TileType type = TileType::EMPTY;
    unsigned int number = 0;
    bool isBomb = false;
};
