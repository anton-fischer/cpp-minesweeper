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
    ui->lbl_title->setText(QString::fromStdString(Quest::questTypeToString(quest->getType())));
    ui->lbl_reward->setText(QString("[%1XP]").arg(quest->getReward()));

    ui->lbl_rarity->setText("SR"); // TODO implement support for this
    ui->lbl_rarity->setStyleSheet("color: #FFC800;");

    ui->progressBar->setMaximum(quest->getGoal());
    ui->progressBar->setValue(quest->getProgress());
}

void QuestElement::on_btn_reroll_clicked()
{
    quest->regenerateQuest();
    updateQuest();
}
