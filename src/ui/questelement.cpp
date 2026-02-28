#include "ui/questelement.h"
#include "ui_questelement.h"

#include "core/quest.h"

QuestElement::QuestElement(Quest* quest, QWidget* parent)
    : quest(quest), QWidget(parent)
    , ui(new Ui::QuestElement)
{
    ui->setupUi(this);

    updateQuest();
}

QuestElement::~QuestElement()
{
    delete ui;
}

void QuestElement::updateQuest()
{
    ui->lbl_title->setText(quest->generateObjectiveString());
    ui->lbl_reward->setText(QString("[%1XP]").arg(quest->getReward()));

    static_assert(QUEST_RARITY_ENUM_GUARD == static_cast<int>(QuestRarity::_END), "QuestRarity enum version mismatch");
    switch (quest->getRarity())
    {
        case QuestRarity::COMMON: {
            ui->lbl_rarity->setText("C");
            ui->lbl_rarity->setStyleSheet("color: #20d300;");
            break;
        }
        case QuestRarity::RARE: {
            ui->lbl_rarity->setText("R");
            ui->lbl_rarity->setStyleSheet("color: #0083d3;");
            break;
        }
        case QuestRarity::EPIC: {
            ui->lbl_rarity->setText("E");
            ui->lbl_rarity->setStyleSheet("color: #b700d3;");
            break;
        }
        case QuestRarity::LEGENDARY: {
            ui->lbl_rarity->setText("SR");
            ui->lbl_rarity->setStyleSheet("color: #FFC800;");
            break;
        }
        default: assert(false);
    }

    ui->progressBar->setMaximum(quest->getGoal());
    ui->progressBar->setValue(quest->getProgress());
}

void QuestElement::on_btn_reroll_clicked()
{
    quest->regenerateQuest();
    updateQuest();
}
