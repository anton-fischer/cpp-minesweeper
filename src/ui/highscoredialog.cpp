#include "ui/highscoredialog.h"
#include "ui_highscoredialog.h"

#include <QLabel>

#include "core/settings.h"
#include "core/highscore.h"

HighscoreDialog::HighscoreDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HighscoreDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Highscores");

    // TODO load highscores from file

    // Header erstellen
    ui->gbx_highscores->addWidget(new QLabel("Name"), highscoreCount, 0);
    ui->gbx_highscores->addWidget(new QLabel("Score"), highscoreCount, 1);
    ui->gbx_highscores->addWidget(new QLabel("Board X"), highscoreCount, 2);
    ui->gbx_highscores->addWidget(new QLabel("Board Y"), highscoreCount, 3);
    ui->gbx_highscores->addWidget(new QLabel("Bomben"), highscoreCount, 4);
    ui->gbx_highscores->addWidget(new QLabel("Seed"), highscoreCount, 5);

    for (auto& highscore : Settings::instance().getHighscores()) {
        addHighscore(&highscore);
        this->highscoreCount++;
    }

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

void HighscoreDialog::addHighscore(Highscore* highscore) {
    ui->gbx_highscores->addWidget(
        new QLabel(highscore->getName()), highscoreCount, 0);

    ui->gbx_highscores->addWidget(
        new QLabel(QString::number(highscore->getScore())), highscoreCount, 1);

    ui->gbx_highscores->addWidget(
        new QLabel(QString::number(highscore->getBoardSizeX())), highscoreCount, 2);

    ui->gbx_highscores->addWidget(
        new QLabel(QString::number(highscore->getBoardSizeY())), highscoreCount, 3);

    ui->gbx_highscores->addWidget(
        new QLabel(QString::number(highscore->getBombCount())), highscoreCount, 4);

    ui->gbx_highscores->addWidget(
        new QLabel(QString::number(highscore->getStartSeed())), highscoreCount, 5);
}
