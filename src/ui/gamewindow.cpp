#include "ui/gamewindow.h"
#include "ui_gamewindow.h"

#include <QMessageBox>
#include <QLayout>
#include <QScrollBar>
#include <QSizePolicy>
#include <QTimer>
#include <QFileDialog>

#include "ui/settingsDialog.h"
#include "ui/enddialog.h"
#include "ui/menuwindow.h"
#include "ui/levelelement.h"

#include "core/settings.h"
#include "core/player.h"

#include <utils/filehandler.h>

GameWindow::GameWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    ui->progressBar_progress->setValue(0);
    ui->vbx_gamelog->setAlignment(Qt::AlignTop);

    this->setWindowTitle("Minesweeper");

    auto& player = Settings::instance().getCurrentPlayer();
    connect(player.get(), &Player::playerLevelUp, this, &GameWindow::playerLevelUp);

    for (auto& quest : player->getQuests()) {
        connect(quest.get(), &Quest::questCompleted, this, &GameWindow::questCompleted);
    }

    // level progress
    auto levelElement = new LevelElement(player.get(), this);
    ui->hbx_level->addWidget(levelElement);

    ui->lbl_xpBonus->setStyleSheet("color: #fdb800;");

    ui->hbx_main->setStretchFactor(ui->vbx_left, 3);
    ui->hbx_main->setStretchFactor(ui->vbx_right, 2);

    showStatusBarMessage("Minesweeper v1.0", 0);

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

// TODO merge boardUpdated into here??
void GameWindow::createNewBoard(Board* newBoard) {
    if (nullptr == newBoard) board = new Board(this);
    else board = newBoard;

    connect(board, &Board::boardUpdated, this, &GameWindow::boardUpdated);
    connect(board, &Board::tileUpdated, this, &GameWindow::tileUpdated);

    connect(board, &Board::bombHit, this, &GameWindow::bombHit);
    connect(board, &Board::gameWon, this, &GameWindow::gameWon);

    boardUpdated();

    showStatusBarMessage("Successfully created new board", 5000);

    this->setWindowTitle(QString("Minesweeper - %1").arg(Settings::difficultyToString(board->getDifficulty())));
    ui->lbl_xpBonus->setText(QString("XP Bonus: %1x XP").arg(Settings::getDifficultyXpMultiplier(board->getDifficulty())));

    //this->resize(this->sizeHint()); // resize in case board size changed
}

void GameWindow::handleTileClick(const unsigned int x, const unsigned int y, const Qt::MouseButton& button) {
    qDebug() << QString("Click registered at: x[%1] y[%2]").arg(x).arg(y);
    auto& player = Settings::instance().getCurrentPlayer();

    // place flag
    if (button == Qt::RightButton || (button == Qt::LeftButton && ui->btn_flag->isChecked())) {
        int flagPlaced = board->placeFlag(x, y);
        if (flagPlaced == 1) { // flag placed
            player->incrementAmountFlagsPlaced(1);
            appendGameLogMessageWithXp("Flag placed", 10);
        } else if (flagPlaced == -1) { // flag removed
            player->decrementAmountFlagsPlaced(1);
            player->decrementXp(10);
        }
    }
    // uncover tile
    else if (button == Qt::LeftButton) {
        unsigned int amountRevealed = board->revealTile(x, y);
        player->incrementAmountTilesUncovered(amountRevealed);
        appendGameLogMessageWithXp("Tiles uncovered", amountRevealed);
    }
}

void GameWindow::appendGameLogMessageWithXp(const QString& message, const unsigned int xp, const bool bold, const unsigned int color) {
    unsigned int xpGain = xp * Settings::getDifficultyXpMultiplier(board->getDifficulty());
    Settings::instance().getCurrentPlayer()->incrementXp(xpGain);
    appendGameLogMessage(message, xpGain, bold, color);
}

void GameWindow::appendGameLogMessage(const QString& message, const unsigned int xp, const bool bold, const unsigned int color) {
    auto* widget = new QWidget(this);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    auto* messageLabel = new QLabel(message, widget);
    messageLabel->setMaximumHeight(20);
    if (color != 0x000000) messageLabel->setStyleSheet(QString("color: #%1;").arg(color, 6, 16, QChar('0')).toUpper()); // value, at least 6 digits, as base 16, fill with 0

    if (bold) {
        QFont f = this->font();
        f.setPointSize(10);
        f.setBold(true);
        messageLabel->setFont(f);
    }

    layout->addWidget(messageLabel);

    if (xp != 0u) {
        auto* xpLabel = new QLabel(QString::fromStdString("+" + std::to_string(xp) + "XP"), widget);

        xpLabel->setMaximumWidth(50);
        xpLabel->setMaximumHeight(20);
        if (color != 0x000000) xpLabel->setStyleSheet(QString("color: #%1;").arg(color, 6, 16, QChar('0')).toUpper()); // value, at least 6 digits, as base 16, fill with 0

        QFont f = this->font();
        f.setPointSize(10);
        f.setBold(true);
        xpLabel->setFont(f);

        layout->addWidget(xpLabel);
    }

    ui->vbx_gamelog->addWidget(widget);

    QScrollBar* vbar = ui->scrollarea_gamelog->verticalScrollBar();
    QTimer::singleShot(100, this, [vbar]() { // without break scrollbar will not scroll to bottom because of dynamically inserted widget
        vbar->setValue(vbar->maximum());
    });
}

void GameWindow::showStatusBarMessage(QString message, unsigned int timeout) const {
    statusBar()->showMessage(message, timeout);

    QTimer::singleShot(timeout, this, [this]() {
        ui->statusbar->showMessage("Minesweeper v1.0");
    });
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
    ui->lcd_flagCount->display(static_cast<int>(board->getFlagCount()));

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
    ui->lcd_flagCount->display(static_cast<int>(board->getFlagCount()));
    ui->lcd_currentScore->display(0);
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
    appendGameLogMessageWithXp("GAME OVER: Bomb hit!", 50, true, 0xd80000);

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
    appendGameLogMessageWithXp("GAME OVER: You win!", 200, true, 0x00d840);

    EndDialog dialog(player.get(), true, this);
    dialog.exec();

    //QMessageBox::information(this, "Game Won!", "Player won the game");
}

void GameWindow::playerLevelUp() {
    auto& player = Settings::instance().getCurrentPlayer();

    appendGameLogMessage(QString("LEVEL UP: %1->%2").arg(player->getLevel() - 1).arg(player->getLevel()), 0, true, 0xfdb800);
    appendGameLogMessage(QString("> XP for next level up: %1XP").arg(player->getMaxXp()), 0, false, 0xfdb800);
}

void GameWindow::questCompleted(Quest* quest) {
    appendGameLogMessageWithXp("QUEST COMPLETED", quest->getReward(), true, 0xfdb800);
    appendGameLogMessage(">" + quest->generateObjectiveString(), 0, false, 0xfdb800);
}

void GameWindow::on_btn_exit_clicked()
{
    this->close();
}

void GameWindow::on_btn_save_clicked()
{
    assert(nullptr != board);

    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Save board"),
        QString(),
        tr("JSON files (*.json)") // tr in case for possible translation later
    );

    if (!filePath.isEmpty()) {
        FileHandler handler;
        handler.saveBoardAsFile(board, filePath.toStdString());
        appendGameLogMessage("Successfully saved current game", 0, true, 0x00d840);

        showStatusBarMessage(QString("Successfully saved board at location %1").arg(filePath), 5000);
    } else {
        qDebug() << "Invalid path given when select location to save file";
    }
}

void GameWindow::on_btn_load_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Load board"),
        QString(),
        tr("JSON files (*.json)")
    );

    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".json")) filePath += ".json";

        FileHandler handler;
        std::unique_ptr<Board> tmp = handler.createBoardFromFile(filePath.toStdString());
        board = tmp.release();

        bool valid = false;
        if (board->getDifficulty() == Difficulty::CUSTOM) valid = Settings::instance().setSettings(board->getBoardSizeX(), board->getBoardSizeY(), board->getBombCount());
        else                                              valid = Settings::instance().setSettings(board->getDifficulty());

        assert(valid == true); // must be true, otherwise corrupted savefile

        createNewBoard(board);
        appendGameLogMessage("Successfully loaded game", 0, true, 0x00d840);

        showStatusBarMessage(QString("Successfully loaded board"), 5000);
    } else {
        qDebug() << "Invalid path given when select savefile to load";
    }
}
