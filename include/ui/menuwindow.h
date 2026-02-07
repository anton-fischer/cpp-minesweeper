#pragma once

#include <QMainWindow>

class Player;
class Quest;

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget* parent = nullptr);
    ~MenuWindow();

private slots:
    void on_btn_load_clicked();
    void on_btn_save_clicked();
    void on_btn_exit_clicked();
    void on_btn_configure_clicked();
    void on_btn_play_clicked();

private:
    Ui::MenuWindow *ui;

    void closeEvent(QCloseEvent* event) override;

    void loadPlayer(Player* player);
    void savePlayer(Player* player);

    void appendQuest(Quest* quest);
};
