#include "ui/gamewindow.h"
#include "ui_gamewindow.h"

#include <QMessageBox>
#include <QLayout>
#include <QScrollBar>
#include <QSizePolicy>

#include "ui/settingsDialog.h"
#include "ui/enddialog.h"
#include "ui/menuwindow.h"
#include "core/settings.h"
#include "core/player.h"

GameWindow::GameWindow(Player* player, QWidget* parent) : player(player), QMainWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    ui->progressBar_progress->setValue(0);
    ui->gamelog_container->setAlignment(Qt::AlignTop);

    this->setWindowTitle("Minesweeper");

    createNewBoard();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "GameWindow was closed";

    MenuWindow* window = new MenuWindow();
    window->setAttribute(Qt::WA_DeleteOnClose); // avoid potential memory leak
    window->show();

    QMainWindow::closeEvent(event);
}

void GameWindow::createNewBoard() {
    delete board;
    board = new Board(this);

    connect(board, &Board::boardUpdated, this, &GameWindow::boardUpdated);
    connect(board, &Board::tileUpdated, this, &GameWindow::tileUpdated);

    connect(board, &Board::bombHit, this, &GameWindow::bombHit);
    connect(board, &Board::gameWon, this, &GameWindow::gameWon);

    boardUpdated();
}

void GameWindow::handleTileClick(const unsigned int x, const unsigned int y, const Qt::MouseButton& button) {
    qDebug() << QString("Click registered at: x[%1] y[%2]").arg(x).arg(y);

    if (button == Qt::RightButton || (button == Qt::LeftButton && ui->btn_flag->isChecked())) board->placeFlag(x, y);
    else if (button == Qt::LeftButton) board->revealTile(x, y);
}

void GameWindow::appendGameLogMessage(const std::string& message, const unsigned int xp) {
    auto* widget = new QWidget(this);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    auto* messageLabel = new QLabel(QString::fromStdString(message), widget);
    messageLabel->setMaximumHeight(20);

    auto* xpLabel = new QLabel(QString::fromStdString("+" + std::to_string(xp) + "XP"), widget);
    QFont f = this->font();
    f.setPointSize(10);
    f.setBold(true);
    xpLabel->setFont(f);
    xpLabel->setMaximumWidth(50);
    xpLabel->setMaximumHeight(20);

    layout->addWidget(messageLabel);
    layout->addWidget(xpLabel);

    ui->gamelog_container->addWidget(widget);

    QScrollBar* vbar = ui->scrollArea_gamelog->verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

void GameWindow::on_btn_restart_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();

    if (dialog.getSuccess()) createNewBoard();
}

void GameWindow::tileUpdated(const unsigned int x, unsigned int y) {
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

    ui->progressBar_progress->setValue(board->getProgress());
    ui->lcd_flag_count->display(static_cast<int>(board->getFlagCount()));

    //qDebug() << QString("Tile at x[%1] y[%2] updated to text [%3]").arg(x).arg(y).arg(btn->text());
}

void GameWindow::boardUpdated() {
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

    ui->progressBar_progress->setMaximum(sizeX * sizeY);
    ui->lcd_flag_count->display(static_cast<int>(board->getFlagCount()));
    ui->btn_status->setText("😊");

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

    ui->grid->setDisabled(false);
    qDebug() << "Finished updating board grid";
}

void GameWindow::bombHit() {
    ui->grid->setDisabled(true);
    ui->btn_status->setText("😖");

    assert(nullptr != player);
    EndDialog dialog(player, false, this);
    dialog.exec();

    //QMessageBox::information(this, "Game Lost!", "Player hit a bomb");
}

void GameWindow::gameWon() {
    ui->grid->setDisabled(true);
    ui->btn_status->setText("😎");

    assert(nullptr != player);
    EndDialog dialog(player, true, this);
    dialog.exec();

    //QMessageBox::information(this, "Game Won!", "Player won the game");
}

void GameWindow::on_btn_exit_clicked()
{
    this->close();
}

void GameWindow::on_btn_help_clicked()
{
    appendGameLogMessage("asdasdasd", 10u);
}

