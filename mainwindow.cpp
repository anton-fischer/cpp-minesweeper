#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QLayout>
#include <QSizePolicy>

#include "settingsDialog.h"
#include "settings.h"
#include "player.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    ui->progressbar->setValue(0);
    ui->scrollboards_container->setAlignment(Qt::AlignTop);

    this->setWindowTitle("Minesweeper");

    createNewBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createNewBoard() {
    delete board;
    board = new Board(this);

    connect(board, &Board::boardUpdated, this, &MainWindow::boardUpdated);
    connect(board, &Board::tileUpdated, this, &MainWindow::tileUpdated);

    connect(board, &Board::bombHit, this, &MainWindow::bombHit);
    connect(board, &Board::gameWon, this, &MainWindow::gameWon);

    boardUpdated();
}

void MainWindow::handleTileClick(const unsigned int x, const unsigned int y, const Qt::MouseButton& button) {
    qDebug() << QString("Click registered at: x[%1] y[%2]").arg(x).arg(y);

    if (button == Qt::LeftButton) board->revealTile(x, y);
    else if (button == Qt::RightButton) board->placeFlag(x, y);
}

void MainWindow::appendScoreBoard(Player& player) {
    auto* widget = new QWidget(this);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    auto* nameLabel = new QLabel(QString::fromStdString(player.getName()), widget);
    nameLabel->setMaximumWidth(50);
    nameLabel->setMaximumHeight(20);

    auto* scoreDisplay = new QLCDNumber(widget);
    scoreDisplay->display(static_cast<int>(player.getScore()));
    scoreDisplay->setDigitCount(5);
    scoreDisplay->setMaximumHeight(20);

    layout->addWidget(nameLabel);
    layout->addWidget(scoreDisplay);

    ui->scrollboards_container->addWidget(widget);
}

void MainWindow::on_pushButton_2_clicked()
{
    Player p;
    appendScoreBoard(p);
}

void MainWindow::on_btn_restart_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();

    if (dialog.getSuccess()) createNewBoard();
}

void MainWindow::tileUpdated(const unsigned int x, unsigned int y) {
    QToolButton* btn = boardGridTiles[y][x];
    const Tile& tile = board->getTile(x, y);

    if (tile.getIsFlag()) {
        btn->setText("🚩");
    }
    else if (!tile.getIsCovered() && tile.getIsBomb()) {
        btn->setText("💣");
        btn->setEnabled(false);
    }
    else if (!tile.getIsCovered()) {
        btn->setText(QString::number(tile.getNumber()));
        btn->setEnabled(false);
    }
    else {
        btn->setText("");
        btn->setEnabled(true);
    }

    ui->progressbar->setValue(board->getProgress());
    ui->lcd_flag_count->display(static_cast<int>(board->getFlagCount()));

    //qDebug() << QString("Tile at x[%1] y[%2] updated to text [%3]").arg(x).arg(y).arg(btn->text());
}

void MainWindow::boardUpdated() {
    // delete old grid
    if (nullptr != boardGrid) {
        QLayoutItem* item;
        while (nullptr != (item = boardGrid->takeAt(0))) {
            delete item->widget();
            delete item;
        }
        delete boardGrid;
    }

    // create new grid
    boardGrid = new QGridLayout(ui->grid);
    boardGrid->setSpacing(0);
    boardGrid->setContentsMargins(0,0,0,0);
    boardGrid->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);

    const unsigned int sizeX = Settings::instance().getBoardSizeX();
    const unsigned int sizeY = Settings::instance().getBoardSizeY();

    ui->progressbar->setMaximum(sizeX * sizeY);
    ui->lcd_flag_count->display(static_cast<int>(board->getFlagCount()));

    boardGridTiles.clear();
    boardGridTiles.resize(sizeY, std::vector<TileButton*>(sizeX, nullptr));

    for (unsigned int y = 0; y < sizeY; ++y) {
        for (unsigned int x = 0; x < sizeX; ++x) {

            TileButton* btn = new TileButton(this);

            btn->setProperty("x", x);
            btn->setProperty("y", y);

            boardGrid->addWidget(btn, y, x);
            boardGridTiles[y][x] = btn;

            // add click handler
            connect(btn, &TileButton::tileClicked, this, [=](Qt::MouseButton button) {
                handleTileClick(x, y, button);
            });

            tileUpdated(x, y);
        }
    }

    qDebug() << "Finished updating board grid";
}

void MainWindow::bombHit() {
    QMessageBox::information(this, "Game Lost!", "Player hit a bomb");
}

void MainWindow::gameWon() {
    QMessageBox::information(this, "Game Won!", "Player won the game");
}

void MainWindow::on_btn_exit_clicked()
{
    QCoreApplication::quit();
}
