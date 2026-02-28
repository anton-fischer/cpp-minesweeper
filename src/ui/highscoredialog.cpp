#include "ui/highscoredialog.h"
#include "ui_highscoredialog.h"

#include <QLabel>
#include <QToolButton>

#include "core/settings.h"
#include "core/highscore.h"

HighscoreDialog::HighscoreDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HighscoreDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Highscores");

    for (auto& highscore : Settings::instance().getHighscores()) {
        addHighscore(&highscore);
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
    QGridLayout* grid = ui->gbx_highscores;

    unsigned int row = grid->rowCount();

    // Column 1: Rank
    QLabel* lblRank = new QLabel(QString::number(row));
    lblRank->setAlignment(Qt::AlignCenter);
    grid->addWidget(lblRank, row, 0);

    // Column 2: Score
    QLabel* lblScore = new QLabel(QString::number(highscore->getScore()));
    lblScore->setAlignment(Qt::AlignCenter);
    grid->addWidget(lblScore, row, 1);

    // Column 3: Name
    QLabel* lblName = new QLabel(highscore->getName());
    lblName->setAlignment(Qt::AlignCenter);
    grid->addWidget(lblName, row, 2);

    // Column 4: Settings
    QString settings = QString("%1x%2 | %3 Bombs")
                           .arg(highscore->getBoardSizeX())
                           .arg(highscore->getBoardSizeY())
                           .arg(highscore->getBombCount());

    QLabel* lblSettings = new QLabel(settings);
    lblSettings->setAlignment(Qt::AlignCenter);
    grid->addWidget(lblSettings, row, 3);

    // Column 5: Replay Button
    QToolButton* btnReplay = new QToolButton();
    btnReplay->setText("🔄 Replay");
    btnReplay->setMinimumSize(30, 30);
    grid->addWidget(btnReplay, row, 4);

    connect(btnReplay, &QToolButton::clicked, this, [highscore]() {
        qDebug() << "Replay clicked for seed:" << highscore->getStartSeed();
        // TODO implement replay logic
    });

    qDebug() << "Added highscore: " << *highscore;
}
