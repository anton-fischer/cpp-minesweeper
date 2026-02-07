#include "ui/enddialog.h"
#include "ui_enddialog.h"

#include <QToolButton>

#include "ui/gamewindow.h"
#include "core/player.h"
#include "core/quest.h"

EndDialog::EndDialog(Player* player, bool gameWon, QWidget *parent) : player(player), gameWon(gameWon), QDialog(parent), ui(new Ui::EndDialog)
{
    ui->setupUi(this);

    connect(this, &QDialog::rejected, this, []() {
        qDebug() << "Dialog was closed via X";
        // TODO
    });

    if (gameWon) {
        this->setWindowTitle("Game Won!");

        ui->lbl_title->setText("GAME WON");
        ui->lbl_subtitle->setText("All bombs cleared!");

    } else {
        this->setWindowTitle("Game Over!");

        ui->lbl_title->setText("GAME OVER");
        ui->lbl_subtitle->setText("A bomb got hit!");
    }

    for (auto& quest : player->getQuests()) {
        appendQuest(quest.get());
    }
}

void EndDialog::appendQuest(Quest* quest) {
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

EndDialog::~EndDialog()
{
    delete ui;
}

void EndDialog::on_btn_finish_clicked()
{
    if (auto parent = parentWidget()) {
        parent->close();
    }
    close();
}

void EndDialog::on_btn_replay_clicked()
{
    if (auto parent = qobject_cast<GameWindow*>(parentWidget())) {
        parent->createNewBoard();
    }
    close();
}
