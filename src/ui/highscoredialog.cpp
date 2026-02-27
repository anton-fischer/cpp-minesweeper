#include "ui/highscoredialog.h"
#include "ui_highscoredialog.h"

#include "core/highscore.h"

HighscoreDialog::HighscoreDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HighscoreDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Highscores");

    // TODO load highscores from file

    connect(this, &QDialog::rejected, this, []() {
        qDebug() << "Dialog was closed via X";

        // TODO save highscores
    });
}

HighscoreDialog::~HighscoreDialog()
{
    delete ui;
}

void HighscoreDialog::on_btn_exit_clicked()
{
    close();
}

void addHighscore(Highscore* highscore) {
    // TODO
}
