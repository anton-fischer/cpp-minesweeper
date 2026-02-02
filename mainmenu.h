#pragma once

#include "quest.h"
#include <QMainWindow>

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
    void on_pushButton_5_clicked();

    void on_btn_save_clicked();

    void on_btn_exit_clicked();

private:
    Ui::MainMenu *ui;

    void appendQuest(Quest& quest);
};
