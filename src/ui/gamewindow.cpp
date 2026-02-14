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

#include <utils/filehandler.h>

GameWindow::GameWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    ui->progressBar_progress->setValue(0);
    ui->gamelog_container->setAlignment(Qt::AlignTop);

    this->setWindowTitle("Minesweeper");

    auto& player = Settings::instance().getCurrentPlayer();
    connect(player.get(), &Player::playerLevelUp, this, &GameWindow::playerLevelUp);
    connect(player.get(), &Player::playerXpChange, this, &GameWindow::playerXpChange);

    for (auto& quest : player->getQuests()) {
        connect(quest.get(), &Quest::questCompleted, this, &GameWindow::questCompleted);
    }

    // level progress
    ui->lbl_currentLevel->setText(QString("Level %1").arg(player->getLevel()));
    ui->lbl_nextLevel->setText(QString("Level %1").arg(player->getLevel() + 1));
    ui->progressBar_currentXp->setMaximum(player->getMaxXp());
    ui->progressBar_currentXp->setValue(player->getCurrentXp());

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

void GameWindow::createNewBoard(Board* newBoard) {
    if (nullptr == newBoard) board = new Board(this);
    else board = newBoard;

    connect(board, &Board::boardUpdated, this, &GameWindow::boardUpdated);
    connect(board, &Board::tileUpdated, this, &GameWindow::tileUpdated);

    connect(board, &Board::bombHit, this, &GameWindow::bombHit);
    connect(board, &Board::gameWon, this, &GameWindow::gameWon);

    boardUpdated();

    this->resize(this->sizeHint()); // resize in case board size changed
}

void GameWindow::handleTileClick(const unsigned int x, const unsigned int y, const Qt::MouseButton& button) {
    qDebug() << QString("Click registered at: x[%1] y[%2]").arg(x).arg(y);
    auto& player = Settings::instance().getCurrentPlayer();

    // place flag
    if (button == Qt::RightButton || (button == Qt::LeftButton && ui->btn_flag->isChecked())) {
        bool flagPlaced = board->placeFlag(x, y);
        if (flagPlaced) {
            player->incrementAmountFlagsPlaced(1);
            appendGameLogMessageWithXp("Flag placed", 10);
        }

        // TODO handle case when flag removed
    }
    // uncover tile
    else if (button == Qt::LeftButton) {
        unsigned amountRevealed = board->revealTile(x, y);
        player->incrementAmountTilesUncovered(amountRevealed);
        appendGameLogMessageWithXp("Tiles uncovered", amountRevealed);
    }
}

void GameWindow::appendGameLogMessageWithXp(const std::string& message, const unsigned int xp, const bool bold) {
    unsigned int xpGain = xp * Settings::getDifficultyXpMultiplier(board->getDifficulty());
    Settings::instance().getCurrentPlayer()->incrementXp(xpGain);
    appendGameLogMessage(message, xpGain, bold);
}

void GameWindow::appendGameLogMessage(const std::string& message, const unsigned int xp, const bool bold) {
    auto* widget = new QWidget(this);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    auto* messageLabel = new QLabel(QString::fromStdString(message), widget);
    messageLabel->setMaximumHeight(20);

    if (bold) {
        QFont f = this->font();
        f.setPointSize(10);
        f.setBold(true);
        messageLabel->setFont(f);
    }

    layout->addWidget(messageLabel);

    if (xp != 0u) {
        auto* xpLabel = new QLabel(QString::fromStdString("+" + std::to_string(xp) + "XP"), widget);
        QFont f = this->font();
        f.setPointSize(10);
        f.setBold(true);
        xpLabel->setFont(f);
        xpLabel->setMaximumWidth(50);
        xpLabel->setMaximumHeight(20);

        layout->addWidget(xpLabel);
    }

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

    const unsigned int sizeX = board->getBoardSizeX();
    const unsigned int sizeY = board->getBoardSizeY();

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
    auto& player = Settings::instance().getCurrentPlayer();

    ui->grid->setDisabled(true);
    ui->btn_status->setText("😖");

    player->incrementAmountBombsHit(1);
    player->incrementAmountGamesPlayed(1);

    // xp and log message
    appendGameLogMessageWithXp("GAME OVER: Bomb hit!", 50, true);

    EndDialog dialog(player.get(), false, this);
    dialog.exec();

    //QMessageBox::information(this, "Game Lost!", "Player hit a bomb");
}

void GameWindow::gameWon() {
    auto& player = Settings::instance().getCurrentPlayer();

    ui->grid->setDisabled(true);
    ui->btn_status->setText("😎");

    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END),  "Difficulty enum version mismatch");

    switch(board->getDifficulty()) {
        case (Difficulty::EASY):   player->incrementAmountEasyGamesWon(1); break;
        case (Difficulty::MEDIUM): player->incrementAmountMediumGamesWon(1); break;
        case (Difficulty::HARD):   player->incrementAmountHardGamesWon(1); break;
        case (Difficulty::CUSTOM): player->incrementAmountCustomGamesWon(1); break;
        default: assert(false);
    }

    player->incrementAmountGamesPlayed(1);

    // xp and log message
    appendGameLogMessageWithXp("GAME OVER: You win!", 200, true);

    EndDialog dialog(player.get(), true, this);
    dialog.exec();

    //QMessageBox::information(this, "Game Won!", "Player won the game");
}

void GameWindow::playerLevelUp() {
    auto& player = Settings::instance().getCurrentPlayer();

    appendGameLogMessage("LEVEL UP: " + std::to_string(player->getLevel() - 1) + "->" + std::to_string(player->getLevel()), 0, true);
    appendGameLogMessage("> XP for next level up: " + std::to_string(player->getMaxXp()) + "XP");

    ui->lbl_currentLevel->setText(QString("Level %1").arg(player->getLevel()));
    ui->lbl_nextLevel->setText(QString("Level %1").arg(player->getLevel() + 1));
    ui->progressBar_currentXp->setMaximum(player->getMaxXp());
}

void GameWindow::playerXpChange() {
    ui->progressBar_currentXp->setValue(Settings::instance().getCurrentPlayer()->getCurrentXp());
}

void GameWindow::questCompleted(Quest* quest) {
    appendGameLogMessageWithXp("QUEST COMPLETED", quest->getReward(), true);
    appendGameLogMessage(">" + quest->generateObjectiveString());
}

void GameWindow::on_btn_exit_clicked()
{
    this->close();
}

void GameWindow::on_btn_save_clicked()
{
    assert(nullptr != board);

    FileHandler handler;
    handler.saveBoardAsFile(board);
}

void GameWindow::on_btn_load_clicked()
{
    FileHandler handler;
    std::unique_ptr<Board> tmp = handler.createBoardFromFile();
    board = tmp.release();

    bool valid = Settings::instance().setSettings(board->getBoardSizeX(), board->getBoardSizeY(), board->getBombCount(), board->getDifficulty());
    assert(valid == true); // must be true, otherwise corrupted savefile

    createNewBoard(board);
}
