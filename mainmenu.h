#pragma once

#include <QMainWindow>

class Player;
class Quest;

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_btn_save_clicked();
    void on_btn_exit_clicked();
    void on_btn_configure_clicked();
    void on_btn_play_clicked();

    void on_btn_load_clicked();

private:
    Ui::MainMenu *ui;

    void loadPlayer(Player* player);
    void savePlayer(Player* player);

    void appendQuest(Quest* quest);
};
