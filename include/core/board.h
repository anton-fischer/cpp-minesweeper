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
    ~Board() = default;

    bool placeFlag(const unsigned int x, const unsigned int y);

    unsigned int revealTile(const unsigned int x, const unsigned int y);
    unsigned int revealAllTiles();

    std::vector<std::vector<Tile>> getBoard() const;
    Tile getTile(const unsigned int x, const unsigned int y) const;

    unsigned int getProgress() const;
    unsigned int getFlagCount() const;

signals:
    void tileUpdated(const unsigned int x, const unsigned int y);
    void boardUpdated();

    void bombHit();
    void gameWon();

private:
    unsigned int boardSizeX;
    unsigned int boardSizeY;
    unsigned int bombCount;

    unsigned int progress = 0;
    unsigned int flagCount = 0;

    bool isInitialized = false;

    void initBoard(const unsigned int startX, const unsigned int startY);
    void generateBombs(const unsigned int startX, const unsigned int startY);
    void generateTileNumbers();

    void handleBombHit(const unsigned int x, const unsigned int y);
    void handleGameWon(const unsigned int x, const unsigned int y);

    std::vector<std::vector<Tile>> board;
};
