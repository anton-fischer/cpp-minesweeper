#include "ui/levelelement.h"
#include "ui_levelelement.h"

#include "core/player.h"

LevelElement::LevelElement(QWidget* parent)
    : player(nullptr), QWidget(parent)
    , ui(new Ui::LevelElement)
{
    ui->setupUi(this);

    // unitialized levelbar if no player is loaded yet
    ui->lbl_current->setText(QString("Level 0"));
    ui->lbl_next->setText(QString("Level 0"));

    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->progressBar->setToolTip(QString("Progress: [0XP/100XP]"));
}

LevelElement::LevelElement(Player* player, QWidget* parent)
    : player(player), QWidget(parent)
    , ui(new Ui::LevelElement)
{
    ui->setupUi(this);

    updatePlayer(player);
}

LevelElement::~LevelElement()
{
    delete ui;
}

void LevelElement::updatePlayer(Player* newPlayer)
{
    assert(nullptr != newPlayer);

    disconnect(player, nullptr, this, nullptr);
    player = newPlayer;

    connect(player, &Player::playerLevelUp, this, &LevelElement::playerLevelUp);
    connect(player, &Player::playerXpChange, this, &LevelElement::playerXpChange);

    ui->lbl_current->setText(QString("Level %1").arg(player->getLevel()));
    ui->lbl_next->setText(QString("Level %1").arg(player->getLevel() + 1));

    ui->progressBar->setMaximum(player->getMaxXp());
    ui->progressBar->setValue(player->getCurrentXp());
    ui->progressBar->setToolTip(QString("Progress: [%1XP/%2XP]").arg(player->getCurrentXp()).arg(player->getMaxXp()));
}

void LevelElement::playerLevelUp()
{
    ui->lbl_current->setText(QString("Level %1").arg(player->getLevel()));
    ui->lbl_next->setText(QString("Level %1").arg(player->getLevel() + 1));

    ui->progressBar->setMaximum(player->getMaxXp());
    ui->progressBar->setToolTip(QString("Progress: [%1XP/%2XP]").arg(player->getCurrentXp()).arg(player->getMaxXp()));
}

void LevelElement::playerXpChange()
{
    ui->progressBar->setValue(player->getCurrentXp());
    ui->progressBar->setToolTip(QString("Progress: [%1XP/%2XP]").arg(player->getCurrentXp()).arg(player->getMaxXp()));
}
