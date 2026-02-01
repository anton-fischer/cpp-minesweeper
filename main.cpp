#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/resources/icons8-minesweeper-96.png"));

    MainWindow w;
    w.show();

    return a.exec();
}
