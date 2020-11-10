#include "mainwindow.h"
#include <appcontroller.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    AppController ac(&w);
    return a.exec();
}
