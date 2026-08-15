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

    ui->progressBar->setMaximum(quest->getGoal());
    ui->progressBar->setValue(quest->getProgress());
    ui->progressBar->setToolTip(QString("Progress: [%1/%2]").arg(quest->getProgress()).arg(quest->getGoal()));

    static_assert(QUEST_RARITY_ENUM_GUARD == static_cast<int>(QuestRarity::_END), "QuestRarity enum version mismatch");
    switch (quest->getRarity())
    {
        case QuestRarity::COMMON: {
            ui->lbl_rarity->setText("C");
            ui->lbl_rarity->setStyleSheet("color: #20d300;");
            ui->wdg_inner->setStyleSheet("#wdg_inner{ background: qlineargradient(x1:1, y1:0, x2:0, y2:0,stop:0 rgba(32,211,0,255),stop:1 rgba(32,211,0,0)); }");
            ui->progressBar->setStyleSheet("QProgressBar{ border: 2px solid #555; border-radius: 5px; text-align: center; } QProgressBar::chunk { background-color: #20d300; width: 20px; }");
            break;
        }
        case QuestRarity::RARE: {
            ui->lbl_rarity->setText("R");
            ui->lbl_rarity->setStyleSheet("color: #0083d3;");
            ui->wdg_inner->setStyleSheet("#wdg_inner{ background: qlineargradient(x1:1, y1:0, x2:0, y2:0,stop:0 rgba(0,131,211,255),stop:1 rgba(0,131,211,0)); }");
            ui->progressBar->setStyleSheet("QProgressBar{ border: 2px solid #555; border-radius: 5px; text-align: center; } QProgressBar::chunk { background-color: #0083d3; width: 20px; }");
            break;
        }
        case QuestRarity::EPIC: {
            ui->lbl_rarity->setText("E");
            ui->lbl_rarity->setStyleSheet("color: #b700d3;");
            ui->wdg_inner->setStyleSheet("#wdg_inner{ background: qlineargradient(x1:1, y1:0, x2:0, y2:0,stop:0 rgba(183,0,211,255),stop:1 rgba(183,0,211,0)); }");
            ui->progressBar->setStyleSheet("QProgressBar{ border: 2px solid #555; border-radius: 5px; text-align: center; } QProgressBar::chunk { background-color: #b700d3; width: 20px; }");
            break;
        }
        case QuestRarity::LEGENDARY: {
            ui->lbl_rarity->setText("SR");
            ui->lbl_rarity->setStyleSheet("color: #FFC800;");
            ui->wdg_inner->setStyleSheet("#wdg_inner{ background: qlineargradient(x1:1, y1:0, x2:0, y2:0,stop:0 rgba(255,200,0,255),stop:1 rgba(255,200,0,0)); }");
            ui->progressBar->setStyleSheet("QProgressBar{ border: 2px solid #555; border-radius: 5px; text-align: center; } QProgressBar::chunk { background-color: #FFC800; width: 20px; }");
            break;
        }
        default: assert(false);
    }
}

void QuestElement::on_btn_reroll_clicked()
{
    quest->regenerateQuest();
    updateQuest();
}
