#pragma once

#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QLCDNumber>

#include <vector>
#include "core/board.h"
#include "ui/tilebutton.h"

class Player;
class Quest;

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = nullptr);
    explicit GameWindow(Board* startBoard, QWidget* parent = nullptr);
    ~GameWindow();

    void createNewBoard(Board* newBoard = nullptr);
    Board* getBoard() const;

    unsigned int getGameScore() const;

private slots:
    void tileUpdated(const unsigned int x, unsigned int y);
    void boardUpdated();

    void bombHit();
    void gameWon();

    void questUpdated(Quest* quest);
    void questCompleted(Quest* quest);

    void playerLevelUp();

    void on_btn_restart_clicked();
    void on_btn_exit_clicked();

    void on_btn_load_clicked();
    void on_btn_save_clicked();

private:
    Ui::GameWindow *ui;

    void closeEvent(QCloseEvent* event) override;

    void handleTileClick(unsigned int x, unsigned int y, const Qt::MouseButton& button);

    // increases player score and increment player xp based on difficulty
    void increaseScore(unsigned int amount);
    // decreases player score and increment player xp based on difficulty
    void decreaseScore(unsigned int amount);

    // shows a file dialog to chose and saves the current board
    // returns true if loading was successful, else false
    bool loadBoard();

    // shows a file dialog to chose and saves the current board
    // returns true if saving was successful, else false
    bool saveBoard();

    // append a message to the gamelog
    void appendGameLogMessage(const QString& message, unsigned int xp = 0, bool bold = false, unsigned int color = 0x000000);

    void showStatusBarMessage(const QString& message, unsigned int timeout) const;

    unsigned int gameScore = 0;

    Board* board = nullptr;
    QGridLayout* boardGrid = nullptr;

    std::vector<std::vector<TileButton*>> boardGridTiles;
};
