/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"
#include "routeplanner_astar.h"
#include "drawablemapwidget.h"
#include <iostream>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    DbManager dbm;
    Routeplanner_astar rpl;

private slots:
    void on_pushButton_clicked();

    void on_flighttable_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QListWidgetItem* selItem;
    void fillTable(QListWidget* list, std::vector<std::vector<int>> routes);
};

#endif // MAINWINDOW_H
