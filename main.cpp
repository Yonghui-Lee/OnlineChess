#include "mainwindow.h"
#include <QApplication>
#include "info.h"

Info *info;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    info = new Info;
    MainWindow w;
    w.initializeNet();

  //  w.show();
    //game->show();
   // game->start();

    return a.exec();
}
