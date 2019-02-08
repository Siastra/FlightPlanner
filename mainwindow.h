#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"
#include "routeplanner.h"
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    DbManager dbm{"../Flugplaner/static/AirlineRoutes.db"};

private:
    Ui::MainWindow *ui;
    DbManager dbm{"../FlightPlanner/static/AirlineRoutes.db"};
    RoutePlanner rpl{&dbm};
};

#endif // MAINWINDOW_H
