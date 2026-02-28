#pragma once

#include <QMainWindow>

#include <core/settings.h>

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

    void on_btn_highscores_clicked();

private:
    Ui::MenuWindow *ui;

    bool isSaveFileSaved = true;

    LevelElement* levelElement = nullptr;

    void closeEvent(QCloseEvent* event) override;

    // shows a file dialog to chose and loads the current player
    // returns true if loading was successful, else false
    bool loadPlayer();

    // updates the UI for the given player
    bool loadPlayerUI(Player* player = Settings::instance().getCurrentPlayer().get());

    // shows a file dialog to chose and saves the current player
    // returns true if saving was successful, else false
    bool savePlayer();

    void appendQuest(Quest* quest);
    void clearQuests();

    void showStatusBarMessage(QString message, unsigned int timeout) const;
};
