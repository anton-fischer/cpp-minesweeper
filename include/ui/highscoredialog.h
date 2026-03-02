#pragma once

#include <QDialog>

class Highscore;

namespace Ui {
class HighscoreDialog;
}

class HighscoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HighscoreDialog(QWidget *parent = nullptr);
    ~HighscoreDialog();

private slots:
    void on_btn_exit_clicked();

private:
    Ui::HighscoreDialog *ui;

    void updateUI();

    void addHighscore(Highscore* highscore);
    void clearHighscores();

    unsigned int highscoreCount = 0;
};
