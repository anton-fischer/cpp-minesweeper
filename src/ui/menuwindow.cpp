#include "ui/menuwindow.h"
#include "ui_menuwindow.h"

#include <QToolButton>
#include <QInputDialog>
#include <QLineEdit>
#include <memory>

#include "ui/gamewindow.h"
#include "ui/settingsdialog.h"
#include "core/settings.h"
#include "core/player.h"
#include "core/quest.h"

MenuWindow::MenuWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    if (auto& currentPlayer = Settings::instance().getCurrentPlayer()) {
        loadPlayer(currentPlayer.get());
    }
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
    auto* widget = new QWidget(this);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    auto* objectiveLabel = new QLabel(QString::fromStdString(quest->generateObjectiveString()), widget);
    objectiveLabel->setMinimumWidth(150);
    objectiveLabel->setMinimumHeight(30);

    auto* progressBar = new QProgressBar(widget);
    progressBar->setMaximum(quest->getGoal());
    progressBar->setValue(quest->getProgress());

    auto* rerollButton = new QToolButton(widget);
    rerollButton->setMinimumWidth(30);
    rerollButton->setMinimumHeight(30);
    rerollButton->setText("🔄️");

    // add click handler
    connect(rerollButton, &QToolButton::clicked, this, [=]() {
        quest->regenerateQuest();

        objectiveLabel->setText(QString::fromStdString(quest->generateObjectiveString()));
        progressBar->setMaximum(quest->getGoal());
        progressBar->setValue(quest->getProgress());
    });

    layout->addWidget(objectiveLabel);
    layout->addWidget(progressBar);
    layout->addWidget(rerollButton);

    ui->quests_container->addWidget(widget);
}

void MenuWindow::loadPlayer(Player* player) {
    // general
    ui->lbl_currentPlayer->setText(QString::fromStdString("Currently playing as: " + player->getName()));

    ui->lbl_currentLevel->setText(QString::fromStdString("Level " + std::to_string(player->getLevel())));
    ui->lbl_nextLevel->setText(QString::fromStdString("Level " + std::to_string(player->getLevel() + 1)));

    ui->progressBar_currentXp->setMaximum(player->getMaxXp());
    ui->progressBar_currentXp->setValue(player->getCurrentXp());

    // stats
    ui->lcd_easyGamesWon->display(static_cast<int>(player->getAmountEasyGamesWon()));
    ui->lcd_mediumGamesWon->display(static_cast<int>(player->getAmountMediumGamesWon()));
    ui->lcd_hardGamesWon->display(static_cast<int>(player->getAmountHardGamesWon()));
    ui->lcd_customGamesWon->display(static_cast<int>(player->getAmountCustomGamesWon()));
    ui->lcd_bombsHit->display(static_cast<int>(player->getAmountBombsHit()));
    ui->lcd_flagsPlaced->display(static_cast<int>(player->getAmountFlagsPlaced()));
    ui->lcd_tilesUncovered->display(static_cast<int>(player->getAmountTilesUncovered()));

    // quests
    for (auto& quest : player->getQuests()) {
        appendQuest(quest.get());
    }
}

void MenuWindow::savePlayer(Player* player) {

}

void MenuWindow::on_btn_save_clicked()
{
    std::unique_ptr<Quest> q = std::make_unique<Quest>();
    appendQuest(q.get());
}

void MenuWindow::on_btn_load_clicked()
{
    std::unique_ptr<Player> p = std::make_unique<Player>();
    Settings::instance().setCurrentPlayer(p);

    loadPlayer(Settings::instance().getCurrentPlayer().get());
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
            std::unique_ptr<Player> p = std::make_unique<Player>(playerName.toStdString());
            Settings::instance().setCurrentPlayer(p);
        } else {
            // user pressed cancel or invalid input
            return;
        }
    }

    GameWindow* window = new GameWindow(Settings::instance().getCurrentPlayer().get());
    window->show();
    this->close();
}

void MenuWindow::on_btn_configure_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();
}
