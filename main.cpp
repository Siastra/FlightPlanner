/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#include "mainwindow.h"
#include <QApplication>

std::vector<Route> DbManager::routes = std::vector<Route>();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbManager{"./AirlineRoutes.db"};
    MainWindow w;
    w.setWindowTitle("FlightPlanner");
    w.show();

    return a.exec();
}
