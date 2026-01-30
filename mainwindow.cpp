#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingsDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressbar->setValue(50);

    auto* layout = new QGridLayout(ui->grid);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    const unsigned int sizeX = 10;
    const unsigned int sizeY = 10;

    board.resize(sizeY, std::vector<QToolButton*>(sizeX, nullptr));

    for (unsigned int y = 0; y < sizeY; ++y) {
        for (unsigned int x = 0; x < sizeX; ++x) {
            QToolButton* btn = new QToolButton(this);
            btn->setProperty("x", x);
            btn->setProperty("y", y);

            btn->setText("X");
            btn->setFixedSize(32, 32);
            btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
            btn->setAutoRaise(false);

            layout->addWidget(btn, x, y);
            board[y][x] = btn;

            // add click handler
            connect(btn, &QToolButton::clicked, this, [=]() {
                handleTileClick(x, y);
            });
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::handleTileClick(const unsigned int x, const unsigned int y) {

}

void MainWindow::appendScoreBoard(const std::string playerName, const unsigned int score) {
    auto* widget = new QWidget(this);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    auto* nameLabel = new QLabel(QString::fromStdString(playerName), widget);
    nameLabel->setMaximumWidth(50);

    auto* scoreDisplay = new QLCDNumber(widget);
    scoreDisplay->display(static_cast<int>(score));
    scoreDisplay->setDigitCount(5);

    layout->addWidget(nameLabel);
    layout->addWidget(scoreDisplay);

    ui->scrollboards_container->addWidget(widget);
}


void MainWindow::on_pushButton_2_clicked()
{
    appendScoreBoard("asdasd", 123);
}


void MainWindow::on_btn_restart_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();
}

