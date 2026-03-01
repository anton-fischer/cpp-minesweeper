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

    updateUI();

    connect(this, &QDialog::rejected, this, []() {
        qDebug() << "Dialog was closed via X";
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

void HighscoreDialog::updateUI() {
    clearHighscores();

    for (auto& highscore : Settings::instance().getHighscores()) {
        addHighscore(&highscore);
    }
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

    connect(btnReplay, &QToolButton::clicked, this, [highscore]() {
        qDebug() << "Replay clicked for highscore with seed:" << highscore->getStartSeed();
        // TODO implement replay logic
    });

    grid->addWidget(btnReplay, row, 4);

    // Column 5: Delete Button
    QToolButton* btnDelete = new QToolButton();
    btnDelete->setText("❌ Delete️");
    btnDelete->setMinimumSize(30, 30);

    connect(btnDelete, &QToolButton::clicked, this, [highscore, this]() {
        qDebug() << "Delete clicked for highscore:" << highscore->getName();
        auto& highscores = Settings::instance().getHighscores();

        highscores.erase(
            std::remove_if(highscores.begin(), highscores.end(),
                [highscore](const Highscore& h) {
                    return h.getName() == highscore->getName() && h.getStartSeed() == highscore->getStartSeed() && h.getScore() == highscore->getScore();
                }),
            highscores.end()
        );

        updateUI();
    });

    grid->addWidget(btnDelete, row, 5);

    qDebug() << "Added highscore: " << *highscore;
}

void HighscoreDialog::clearHighscores() {
    for (int i = ui->gbx_highscores->count() - 1; i >= 0; --i) { // iterate backwarts to prevent moving of indices due to takeAt
        int row, col, rowSpan, colSpan;
        ui->gbx_highscores->getItemPosition(i, &row, &col, &rowSpan, &colSpan);

        if (row > 0) {
            QLayoutItem* item = ui->gbx_highscores->takeAt(i);

            if (QWidget* w = item->widget()) {
                w->deleteLater(); // delete widget
            }

            delete item; // delete layout
        }
    }
}
