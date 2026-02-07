#pragma once

#include <QDialog>

class Player;
class Quest;

namespace Ui {
class EndDialog;
}

class EndDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EndDialog(Player* player, bool gameWon, QWidget *parent = nullptr);
    ~EndDialog();

private slots:
    void on_btn_finish_clicked();

    void on_btn_replay_clicked();

private:
    void appendQuest(Quest* quest);

    Ui::EndDialog *ui;

    Player* player;
    const bool gameWon;
};
