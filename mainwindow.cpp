#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingsDialog.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressbar->setValue(0);

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

    boardUpdated();
}

void MainWindow::handleTileClick(const unsigned int x, const unsigned int y, const Qt::MouseButton& button) {
    qDebug() << QString("Click registered at: x[%1] y[%2]").arg(x).arg(y);

    if (button == Qt::LeftButton) board->revealTile(x, y);
    else if (button == Qt::RightButton) board->placeFlag(x, y);
}

void MainWindow::appendScoreBoard(const std::string& playerName, const unsigned int score) {
    auto* widget = new QWidget(this);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    auto* nameLabel = new QLabel(QString::fromStdString(playerName), widget);
    nameLabel->setMaximumWidth(50);

    auto* scoreDisplay = new QLCDNumber(widget);
    scoreDisplay->display(static_cast<int>(score));
    scoreDisplay->setDigitCount(5);

    layout->addWidget(nameLabel);
    layout->addWidget(scoreDisplay);

    ui->scrollboards_container->addWidget(widget);
}

void MainWindow::on_pushButton_2_clicked()
{
    appendScoreBoard("asdasd", 123);
}

void MainWindow::on_btn_restart_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();

    createNewBoard();
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

    const unsigned int sizeX = Settings::instance().getBoardSizeX();
    const unsigned int sizeY = Settings::instance().getBoardSizeY();

    boardGridTiles.clear();
    boardGridTiles.resize(sizeY, std::vector<TileButton*>(sizeX, nullptr));

    for (unsigned int y = 0; y < sizeY; ++y) {
        for (unsigned int x = 0; x < sizeX; ++x) {

            TileButton* btn = new TileButton(this);
            btn->setFixedSize(32, 32);
            btn->setToolButtonStyle(Qt::ToolButtonTextOnly);

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

void MainWindow::on_btn_exit_clicked()
{
    QCoreApplication::quit();
}
