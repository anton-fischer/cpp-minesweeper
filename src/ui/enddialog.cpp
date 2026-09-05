#include "ui/enddialog.h"
#include "ui_enddialog.h"

#include <QToolButton>

#include "ui/gamewindow.h"
#include "ui/questelement.h"
#include "ui/levelelement.h"

#include "core/player.h"
#include "core/quest.h"
#include "core/settings.h"

EndDialog::EndDialog(Player* player, bool gameWon, QWidget *parent) : QDialog(parent), ui(new Ui::EndDialog), player(player), gameWon(gameWon)
{
    ui->setupUi(this);

    connect(this, &QDialog::rejected, this, []() {
        qDebug() << "Dialog was closed via X";
        // TODO
    });

    // title
    if (gameWon) {
        this->setWindowTitle("Game Won!");

        ui->lbl_title->setText("GAME WON");
        ui->lbl_subtitle->setText("All bombs cleared!");

    } else {
        this->setWindowTitle("Game Over!");

        ui->lbl_title->setText("GAME OVER");
        ui->lbl_subtitle->setText("A bomb got hit!");
    }

    // level progress
    auto levelElement = new LevelElement(player, this);
    ui->hbx_level->addWidget(levelElement);

    // quests
    for (auto& quest : player->getQuests()) {
        appendQuest(quest.get());
    }
}

void EndDialog::appendQuest(Quest* quest) {
    auto newQuest = new QuestElement(quest, this);
    ui->vbx_quests->addWidget(newQuest);
}

EndDialog::~EndDialog()
{
    delete ui;
}

void EndDialog::on_btn_finish_clicked()
{
    if (auto parent = qobject_cast<GameWindow*>(parentWidget())) {
        parent->close();

        if (ui->cbx_saveScore->isChecked()) {
            Highscore score(Settings::instance().getCurrentPlayer().get(), parent->getBoard(), parent->getGameScore());
            Settings::instance().addHighscore(score);
        }
    }
    close();
}

void EndDialog::on_btn_replay_clicked()
{
    if (auto parent = qobject_cast<GameWindow*>(parentWidget())) {
        parent->createNewBoard();

        if (ui->cbx_saveScore->isChecked()) {
            Highscore score(Settings::instance().getCurrentPlayer().get(), parent->getBoard(), parent->getGameScore());
            Settings::instance().addHighscore(score);
        }
    }
    close();
}
