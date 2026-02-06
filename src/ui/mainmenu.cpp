#include "ui/mainmenu.h"
#include "ui_mainmenu.h"

#include <QToolButton>
#include <memory>

#include "ui/mainwindow.h"
#include "ui/settingsdialog.h"
#include "core/player.h"
#include "core/quest.h"

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::appendQuest(Quest* quest) {
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

void MainMenu::loadPlayer(Player* player) {
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

void MainMenu::savePlayer(Player* player) {

}

void MainMenu::on_btn_save_clicked()
{
    std::unique_ptr<Quest> q = std::make_unique<Quest>();
    appendQuest(q.get());
}

void MainMenu::on_btn_load_clicked()
{
    std::unique_ptr<Player> p = std::make_unique<Player>();
    loadPlayer(p.get());
}

void MainMenu::on_btn_exit_clicked()
{
    QCoreApplication::quit();
}

void MainMenu::on_btn_play_clicked()
{
    std::unique_ptr<Player> p = std::make_unique<Player>();

    MainWindow* window = new MainWindow(p.get());
    window->show();
    this->close();
}

void MainMenu::on_btn_configure_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();
}
