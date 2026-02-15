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
    ~GameWindow();

    void createNewBoard(Board* newBoard = nullptr);

private slots:
    void tileUpdated(const unsigned int x, unsigned int y);
    void boardUpdated();

    void bombHit();
    void gameWon();

    void questCompleted(Quest* quest);

    void playerLevelUp();

    void on_btn_restart_clicked();
    void on_btn_exit_clicked();

    void on_btn_load_clicked();
    void on_btn_save_clicked();

private:
    Ui::GameWindow *ui;

    void closeEvent(QCloseEvent* event) override;

    void handleTileClick(const unsigned int x, const unsigned int y, const Qt::MouseButton& button);

    // log message and increment player xp based on difficulty
    void appendGameLogMessageWithXp(const std::string& message, const unsigned int xp = 0, const bool bold = false, const std::string color = "");
    // only log message without changing player xp
    void appendGameLogMessage(const std::string& message, const unsigned int xp = 0, const bool bold = false, const std::string color = "");

    Board* board = nullptr;
    QGridLayout* boardGrid = nullptr;

    std::vector<std::vector<TileButton*>> boardGridTiles;
};
