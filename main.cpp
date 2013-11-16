#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLocale::setDefault(QLocale::Chinese);

    MainWindow w;
    w.resize(800,500);
    QSize size = w.size();
    QDesktopWidget *desktop = QApplication::desktop();
    int dw = (desktop->width() - size.width())/2;
    int dh = (desktop->height() - size.height())/2;
    w.move(dw,dh);

    w.show();

    return a.exec();
}
