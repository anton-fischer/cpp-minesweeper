#pragma once

#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QLCDNumber>

#include <vector>
#include "board.h"
#include "tilebutton.h"

class Player;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void tileUpdated(const unsigned int x, unsigned int y);
    void boardUpdated();

    void bombHit();
    void gameWon();

    void on_btn_restart_clicked();
    void on_btn_help_clicked();
    void on_btn_exit_clicked();

private:
    Ui::MainWindow *ui;

    void createNewBoard();

    void handleTileClick(const unsigned int x, const unsigned int y, const Qt::MouseButton& button);
    void appendGameLogMessage(const std::string& message, const unsigned int xp);

    Board* board = nullptr;
    QGridLayout* boardGrid = nullptr;

    std::vector<std::vector<TileButton*>> boardGridTiles;
    std::vector<QHBoxLayout*> scoreboards;
};
