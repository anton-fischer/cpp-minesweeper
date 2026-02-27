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

    void addHighscore(Highscore* highscore);
};
