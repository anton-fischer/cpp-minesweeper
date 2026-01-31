#pragma once

#include <vector>
#include <QObject>
#include "Tile.h"

class Board : public QObject
{
    Q_OBJECT

public:
    explicit Board(QObject* parent = nullptr); // generate board based on current settings
    Board(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount, QObject* parent = nullptr); // generate board based on values

    bool placeFlag(const unsigned int x, const unsigned int y);
    bool revealTile(const unsigned int x, const unsigned int y);

    std::vector<std::vector<Tile>> getBoard() const;
    Tile getTile(const unsigned int x, const unsigned int y) const;

signals:
    void tileUpdated(const unsigned int x, const unsigned int y);
    void boardUpdated(std::vector<std::vector<Tile>> board);

private:
    unsigned int boardSizeX;
    unsigned int boardSizeY;
    unsigned int bombCount;

    bool isInitialized = false;

    void initBoard(const unsigned int startX, const unsigned int startY);
    void generateBombs(const unsigned int startX, const unsigned int startY);
    void generateTileNumbers();

    std::vector<std::vector<Tile>> board;
};
