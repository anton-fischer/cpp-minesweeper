#include "ui/menuwindow.h"

#include <QApplication>
#include <QFile>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*QFile styleSheet(":/styles/resources/lightstyle.qss");
    if (styleSheet.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleSheet.readAll());
        a.setStyleSheet(style);
    }*/

    a.setWindowIcon(QIcon(":/images/resources/icons8-minesweeper-96.png"));

    MenuWindow w;
    w.show();

    return a.exec();
}
