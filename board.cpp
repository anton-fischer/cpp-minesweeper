#include "board.h"

#include <assert.h>
#include "settings.h"

Board::Board(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount) :
    boardSizeX(boardSizeX), boardSizeY(boardSizeY), bombCount(bombCount) {
    initBoard();
};

Board::Board() :
    boardSizeX(Settings::instance().getBoardSizeX()), boardSizeY(Settings::instance().getBoardSizeY()), bombCount(Settings::instance().getBombCount()) {
    initBoard();
};

void Board::initBoard() {
    assert(bombCount < boardSizeX * boardSizeY);

    board.reserve(boardSizeY);
    for (auto& column : board) {
        column.reserve(boardSizeX);
    }

}
