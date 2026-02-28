#pragma once

#include <vector>
#include <QObject>

#include "Tile.h"
#include "core/settings.h"

class Highscore;

#define SAVE_FILE_VERSION 1

class Board : public QObject
{
    Q_OBJECT

public:
    explicit Board(QObject* parent = nullptr); // generate board based on current settings
    Board(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount, const Difficulty difficulty, QObject* parent = nullptr); // generate board based on values
    ~Board() = default;

    // return -1 if flag removed, 0 if nothing changed, 1 if flag placed
    int placeFlag(const unsigned int x, const unsigned int y);

    // return amount of revealed tiles
    unsigned int revealTile(const unsigned int x, const unsigned int y);
    unsigned int revealAllTiles();

    std::vector<std::vector<Tile>> getBoard() const;
    Tile getTile(const unsigned int x, const unsigned int y) const;

    Difficulty getDifficulty() const;

    unsigned int getBoardSizeX() const;
    unsigned int getBoardSizeY() const;
    unsigned int getBombCount() const;

    unsigned int getProgress() const;
    unsigned int getFlagCount() const;

    unsigned int getStartSeed() const;

    static std::unique_ptr<Board> fromHighscore(const Highscore highscore, QObject* parent = nullptr);

    // persistor
    static std::unique_ptr<Board> fromJson(const nlohmann::json& j, QObject* parent = nullptr);
    nlohmann::json toJson() const;

    // toString
    friend QDebug operator<<(QDebug dbg, const Board& b);

signals:
    void tileUpdated(const unsigned int x, const unsigned int y);
    void boardUpdated();

    void bombHit();
    void gameWon();

private:
    Difficulty difficulty;

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

    unsigned int startSeed = 0;
    std::vector<std::vector<Tile>> board;
};
