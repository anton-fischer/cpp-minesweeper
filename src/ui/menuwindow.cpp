#include "ui/menuwindow.h"
#include "ui_menuwindow.h"

#include <QToolButton>
#include <QInputDialog>
#include <QLineEdit>
#include <QTimer>
#include <QFileDialog>

#include <memory>

#include "ui/gamewindow.h"
#include "ui/settingsdialog.h"
#include "ui/highscoredialog.h"
#include "ui/questelement.h"
#include "ui/levelelement.h"

#include "core/settings.h"
#include "core/player.h"
#include "core/quest.h"

#include "utils/filehandler.h"

MenuWindow::MenuWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Minesweeper");

    levelElement = new LevelElement(this);
    ui->hbx_level->addWidget(levelElement);

    if (auto& currentPlayer = Settings::instance().getCurrentPlayer()) {
        loadPlayer(currentPlayer.get());
    } else {
        ui->btn_save->setDisabled(true);
        ui->lbl_noPlayerQuests->setVisible(true);
        ui->lbl_noPlayerStatistics->setVisible(true);
        ui->scrollarea_quests->setVisible(false);
        ui->scrollarea_statistics->setVisible(false);
    }

    ui->hbx_main->setStretchFactor(ui->vbx_left, 5);
    ui->hbx_main->setStretchFactor(ui->vbx_right, 5);

    showStatusBarMessage("Minesweeper v1.0", 0);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "MenuWindow was closed";
    QMainWindow::closeEvent(event);
}

void MenuWindow::appendQuest(Quest* quest) {
    auto newQuest = new QuestElement(quest, this);
    ui->vbx_quests->addWidget(newQuest);
}

void MenuWindow::clearQuests() {
    QLayout* layout = ui->vbx_quests;

    while (layout->count() > 0) {
        QLayoutItem* item = layout->takeAt(0);

        if (item->widget()) {
            delete item->widget(); // delete widget
        }

        delete item; // delete layout
    }
}

void MenuWindow::showStatusBarMessage(QString message, unsigned int timeout) const {
    statusBar()->showMessage(message, timeout);

    QTimer::singleShot(timeout, this, [this]() {
        ui->statusbar->showMessage("Minesweeper v1.0");
    });
}

void MenuWindow::loadPlayer(Player* player) {
    // general
    ui->gbx_player->setTitle("Currently playing as: " + player->getName());

    // stats
    ui->lcd_easyGamesWon->display(static_cast<int>(player->getAmountEasyGamesWon()));
    ui->lcd_mediumGamesWon->display(static_cast<int>(player->getAmountMediumGamesWon()));
    ui->lcd_hardGamesWon->display(static_cast<int>(player->getAmountHardGamesWon()));
    ui->lcd_customGamesWon->display(static_cast<int>(player->getAmountCustomGamesWon()));
    ui->lcd_bombsHit->display(static_cast<int>(player->getAmountBombsHit()));
    ui->lcd_flagsPlaced->display(static_cast<int>(player->getAmountFlagsPlaced()));
    ui->lcd_tilesUncovered->display(static_cast<int>(player->getAmountTilesUncovered()));

    // quests
    clearQuests();
    for (auto& quest : player->getQuests()) {
        appendQuest(quest.get());
    }

    // progress
    levelElement->updatePlayer(player);

    ui->btn_save->setDisabled(false);
    ui->lbl_noPlayerQuests->setVisible(false);
    ui->lbl_noPlayerStatistics->setVisible(false);
    ui->scrollarea_quests->setVisible(true);
    ui->scrollarea_statistics->setVisible(true);
    showStatusBarMessage(QString("Successfully loaded player %1").arg(player->getName()), 5000);
}

void MenuWindow::on_btn_save_clicked()
{
    auto& currentPlayer = Settings::instance().getCurrentPlayer();
    assert(nullptr != currentPlayer);

    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Save player"),
        QString(),
        tr("JSON files (*.json)") // tr in case for possible translation later
    );

    if (!filePath.isEmpty()) {
        FileHandler handler;
        handler.savePlayerAsFile(currentPlayer.get(), filePath.toStdString());

        showStatusBarMessage(QString("Successfully saved player %1 at location %2").arg(currentPlayer->getName(), filePath), 5000);
    } else {
        qDebug() << "Invalid path given when select location to save file";
    }


}

void MenuWindow::on_btn_load_clicked()
{   
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Load player"),
        QString(),
        tr("JSON files (*.json)")
    );

    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".json")) filePath += ".json";

        FileHandler handler;
        auto player = handler.createPlayerFromFile(filePath.toStdString());

        Settings::instance().setCurrentPlayer(player);

        loadPlayer(Settings::instance().getCurrentPlayer().get());

        showStatusBarMessage(QString("Successfully laoded player"), 5000);
    } else {
        qDebug() << "Invalid path given when select savefile to load";
    }
}

void MenuWindow::on_btn_exit_clicked()
{
    QCoreApplication::quit();
}

void MenuWindow::on_btn_play_clicked()
{    
    // if no player loaded, create new one
    if (nullptr == Settings::instance().getCurrentPlayer()) {
        bool ok;
        QString playerName = QInputDialog::getText(
            this,
            "Create Player",
            "Please enter a name:",
            QLineEdit::Normal,
            "",
            &ok
            );

        if (ok && !playerName.isEmpty()) {
            // user pressed okay and entered something
            std::unique_ptr<Player> p = std::make_unique<Player>(playerName);
            Settings::instance().setCurrentPlayer(p);
        } else {
            // user pressed cancel or invalid input
            return;
        }
    }

    GameWindow* window = new GameWindow();
    window->show();
    this->close();
}

void MenuWindow::on_btn_configure_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();
}

void MenuWindow::on_btn_highscores_clicked()
{
    HighscoreDialog dialog(this);
    dialog.exec();
}
