#include "mainwindow.h"
#include "Engine.h"

#include <QApplication>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

