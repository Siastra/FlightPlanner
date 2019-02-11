#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbManager{"../Flugplaner/static/AirlineRoutes.db"};
    MainWindow w;
    w.show();

    return a.exec();
}
