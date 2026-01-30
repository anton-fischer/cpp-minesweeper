#pragma once

#include <QMainWindow>
#include <QToolButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QLCDNumber>

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_btn_restart_clicked();

private:
    Ui::MainWindow *ui;

    void handleTileClick(const unsigned int x, const unsigned int y);

    void appendScoreBoard(const std::string playerName, const unsigned int score);

    std::vector<std::vector<QToolButton*>> board;
    std::vector<QHBoxLayout*> scoreboards;
};
