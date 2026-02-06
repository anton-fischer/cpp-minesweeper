#include "core/board.h"

#include <QDebug>

#include <assert.h>
#include <random>
#include <algorithm>
#include "core/settings.h"

Board::Board(QObject* parent) :
    Board(Settings::instance().getBoardSizeX(), Settings::instance().getBoardSizeY(), Settings::instance().getBombCount(), parent) {
};


Board::Board(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount, QObject* parent) :
    boardSizeX(boardSizeX), boardSizeY(boardSizeY), bombCount(bombCount), QObject(parent) {

    //assert((boardSizeX >= 3 && boardSizeY >= 3 && boardSizeX <= 10 && boardSizeY <= 10) && "invalid boardSize detected while generating board");
    //assert((bombCount >= 1 && bombCount < boardSizeX * boardSizeY / 2) && "invalid bombCount detected while generating board");

    // generate empty tiles
    board.resize(boardSizeY);
    for (auto& column : board) {
        column.resize(boardSizeX);
    }

    flagCount = bombCount;
};

void Board::initBoard(const unsigned int startX, const unsigned int startY) {
    assert((startX < boardSizeX && startY < boardSizeY) && "invalid start position detected while generating board");

    generateBombs(startX, startY);
    generateTileNumbers();

    isInitialized = true;

    qDebug() << "Finished generating board";
    emit boardUpdated();
}

void Board::generateBombs(const unsigned int startX, const unsigned int startY) {
    // shuffle fields and take first N (= bombCount) fields as bombs
    std::vector<std::pair<unsigned int, unsigned int>> positions;

    for (unsigned int y = 0; y < boardSizeY; ++y) {
        for (unsigned int x = 0; x < boardSizeX; ++x) {
            if (std::abs(static_cast<int>(x) - static_cast<int>(startX)) <= 1 && std::abs(static_cast<int>(y) - static_cast<int>(startY)) <= 1) continue; // no bomb allowed on start tile or on neighbour tile
            positions.emplace_back(x, y);
        }
    }

    // shuffle
    std::random_device seed;
    std::mt19937 generator(seed());
    std::shuffle(positions.begin(), positions.end(), generator);

    // place bombs on first N positions
    for (unsigned int i = 0; i < bombCount; ++i) {
        auto [x, y] = positions[i];
        board[y][x].setIsBomb(true);
    }

    qDebug() << "Successfully placed bombs";
}

void Board::generateTileNumbers() {
    // goes through every tile in board and increments neighbours of bomb fields
    for (unsigned int y = 0; y < boardSizeY; ++y) {
        for (unsigned int x = 0; x < boardSizeX; ++x) {
            if (board[y][x].getIsBomb()) { // in case of bomb increment every neighbour tile
                for (int row = -1; row <= 1; ++row) {
                    for (int col = -1; col <= 1; ++col) {
                        if (row == 0 && col == 0) {
                            board[y][x].setNumber(10); // set bomb files to value > 9
                            continue;
                        }

                        const int newX = x + row;
                        const int newY = y + col;

                        if (newX >= 0 && newY >= 0 && newX < boardSizeX && newY < boardSizeY && !board[newY][newX].getIsBomb()) {
                            board[newY][newX].incrementNumber();
                        }
                    }
                }
            }
        }
    }

    qDebug() << "Successfully generated tile numbers";
}

void Board::placeFlag(const unsigned int x, const unsigned int y) {
    if (board[y][x].getIsFlag()) {
        board[y][x].setIsFlag(false);
        progress--;
        flagCount++;
    } else if (flagCount == 0) { // in this case cancel, as no new flag can be placed
        return;
    } else {
        board[y][x].setIsFlag(true);
        progress++;
        flagCount--;
    }

    if (progress >= boardSizeX * boardSizeY) {
        handleGameWon(x, y);
        return;
    }

    emit tileUpdated(x, y);
}

unsigned int Board::revealTile(const unsigned int x, const unsigned int y) {
    if (!isInitialized) initBoard(x, y);
    if (!board[y][x].getIsCovered() || board[y][x].getIsFlag()) return 0;

    if (board[y][x].getIsBomb()) {
        handleBombHit(x, y);
        return 1;
    }

    board[y][x].setIsCovered(false);
    progress++;
    emit tileUpdated(x, y);

    unsigned int uncoveredCount = 1; // this field

    if (board[y][x].getNumber() == 0) { // in case of 0 also uncover every neighbour tile
        for (int row = -1; row <= 1; ++row) {
            for (int col = -1; col <= 1; ++col) {
                const int newX = x + row;
                const int newY = y + col;

                if (newX >= 0 && newY >= 0 && newX < boardSizeX && newY < boardSizeY && board[newY][newX].getIsCovered()) {
                    uncoveredCount += revealTile(newX, newY);
                }
            }
        }
    }

    if (progress >= boardSizeX * boardSizeY) {
        handleGameWon(x, y);
    }

    return uncoveredCount;
}

unsigned int Board::revealAllTiles() {
    unsigned int uncoveredCount = 0;

    for (unsigned int y = 0; y < boardSizeY; ++y) {
        for (unsigned int x = 0; x < boardSizeX; ++x) {
            if (board[y][x].getIsCovered()) {
                board[y][x].setIsCovered(false);
                emit tileUpdated(x, y);
                uncoveredCount++;
            }
        }
    }

    return uncoveredCount;
}

void Board::handleBombHit(const unsigned int x, const unsigned int y) {
    qDebug() << "Bomb hit detected";

    emit tileUpdated(x, y);

    revealAllTiles();

    emit bombHit();
}

void Board::handleGameWon(const unsigned int x, const unsigned int y) {
    qDebug() << "Game won detected";

    emit tileUpdated(x, y);

    revealAllTiles();

    emit gameWon();
}

std::vector<std::vector<Tile>> Board::getBoard() const {
    return this->board;
}

Tile Board::getTile(const unsigned int x, const unsigned int y) const {
    return this->board.at(y).at(x);
}

unsigned int Board::getProgress() const {
    return this->progress;
}

unsigned int Board::getFlagCount() const {
    return this->flagCount;
}
