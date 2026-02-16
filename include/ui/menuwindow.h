#pragma once

#include <QMainWindow>

class Player;
class Quest;
class LevelElement;

QT_BEGIN_NAMESPACE
namespace Ui {
class MenuWindow;
}
QT_END_NAMESPACE

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

    LevelElement* levelElement = nullptr;

    void closeEvent(QCloseEvent* event) override;

    void loadPlayer(Player* player);

    void appendQuest(Quest* quest);
    void clearQuests();

    void showStatusBarMessage(QString message, unsigned int timeout) const;
};
