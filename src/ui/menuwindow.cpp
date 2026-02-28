#include "ui/menuwindow.h"
#include "ui_menuwindow.h"

#include <QToolButton>
#include <QInputDialog>
#include <QLineEdit>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

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
        loadPlayerUI();

        // this case happens, when player returns to main menu -> save file was changed
        this->isSaveFileSaved = false;
    } else {
        ui->lbl_noPlayerQuests->setVisible(true);
        ui->lbl_noPlayerStatistics->setVisible(true);
        ui->scrollarea_quests->setVisible(false);
        ui->scrollarea_statistics->setVisible(false);
    }

    ui->btn_save->setDisabled(this->isSaveFileSaved);

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
    if (isSaveFileSaved) {
        qDebug() << "MenuWindow was closed, savefile was already saved";
        event->accept();
        return;
    }

    QMessageBox::StandardButton choice = QMessageBox::question(
        this,
        tr("Save player profile?"),
        tr("Do you want to save your player profile before exiting?"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
    );

    switch (choice) {
        case QMessageBox::Yes: {
            if (savePlayer()) {
                qDebug() << "MenuWindow was closed, savefile was saved";
                event->accept();
            } else {
                qDebug() << "MenuWindow exit cancelled, savefile save failed";
                event->ignore();
            }
            break;
        }
        case QMessageBox::No: {
            qDebug() << "MenuWindow was closed, savefile was not saved";
            event->accept();
            break;
        }
        case QMessageBox::Cancel:
        default: {
            qDebug() << "MenuWindow exit cancelled";
            event->ignore();
            break;
        }
    }
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

bool MenuWindow::loadPlayer() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Load player"),
        QString(),
        tr("JSON files (*.json)") // tr for possible translation later
    );

    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".json")) filePath += ".json";

        FileHandler handler;
        auto player = handler.createPlayerFromFile(filePath.toStdString());
        if (nullptr == player) {
            qDebug() << "Parsing of savefile was not successful when loading player";
            return false;
        }

        Settings::instance().setCurrentPlayer(std::move(player));
        loadPlayerUI();

        qDebug() << QString("Successfully loaded player %1 from location %2").arg(Settings::instance().getCurrentPlayer()->getName(), filePath);
        return true;
    } else {
        qDebug() << "Invalid path given when select savefile to load";
    }

    return false;
}

bool MenuWindow::loadPlayerUI(Player* player) {
    assert(nullptr != player);

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

    return true;
}

bool MenuWindow::savePlayer() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Save player"),
        QString(),
        tr("JSON files (*.json)") // tr for possible translation later
    );

    if (!filePath.isEmpty()) {
        FileHandler handler;
        handler.savePlayerAsFile(Settings::instance().getCurrentPlayer().get(), filePath.toStdString());

        this->isSaveFileSaved = true;
        // disabled because a player should always be able to save additional copies of the savefile
        //ui->btn_save->setDisabled(this->isSaveFileSaved);

        qDebug() << QString("Successfully saved player %1 at location %2").arg(Settings::instance().getCurrentPlayer()->getName(), filePath);
        return true;
    } else {
        qDebug() << "Invalid path given when select location to save file";
    }

    return false;
}

void MenuWindow::on_btn_save_clicked()
{
    bool success = savePlayer();
    if (success) {
        showStatusBarMessage(QString("Successfully saved player %1").arg(Settings::instance().getCurrentPlayer()->getName()), 5000);
    } else {
        showStatusBarMessage(QString("An error occured while saving player %1").arg(Settings::instance().getCurrentPlayer()->getName()), 5000);
    }
}

void MenuWindow::on_btn_load_clicked()
{
    bool success = loadPlayer();
    if (success) {
        showStatusBarMessage(QString("Successfully loaded player %1").arg(Settings::instance().getCurrentPlayer()->getName()), 5000);
    } else {
        showStatusBarMessage(QString("An error occured while loading player"), 5000);
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
            Settings::instance().setCurrentPlayer(std::make_unique<Player>(playerName));
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
