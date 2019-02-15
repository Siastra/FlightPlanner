/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QCompleter>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    rpl{&dbm},
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList airlines = dbm.getAllAirlineNames(); 
    QCompleter *completerAl = new QCompleter(airlines, this);
    completerAl->setCaseSensitivity(Qt::CaseInsensitive);
    completerAl->setFilterMode(Qt::MatchContains);
    completerAl->setMaxVisibleItems(20);
    ui->airlineSearch->setCompleter(completerAl);

    QStringList airports = dbm.getAllAirportNames();
    QCompleter *completerAp = new QCompleter(airports, this);
    completerAp->setCaseSensitivity(Qt::CaseInsensitive);
    completerAp->setFilterMode(Qt::MatchContains);
    completerAp->setMaxVisibleItems(20);
    ui->FromSearch->setCompleter(completerAp);
    ui->ToSearch->setCompleter(completerAp);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    auto from_id{dbm.getAirportIdFromInput(ui->FromSearch->text().toStdString())};
    auto to_id{dbm.getAirportIdFromInput(ui->ToSearch->text().toStdString())};

    //std::cout << rpl.get_min_hops(4908, 3699) << std::endl;  // vienna to palm springs (should be 4 hops)
    //std::cout << rpl.get_min_hops(from_id, to_id) << std::endl;

    std::cout << rpl.get_min_hops(from_id, to_id) << std::endl;
    auto routes = rpl.get_routes(from_id, to_id);
    size_t min = 1000;
    for (auto route : routes) {
        if (route.size() < min) {
            min = route.size();
        }
    }
    ui->map->connectTheDots(routes);
    fillTable(ui->flighttable, routes);

    //auto routes{rpl.get_routes(from_id, to_id)};
    //rpl.print_list_list(routes);
}

void MainWindow::fillTable(QListWidget *list, std::vector<std::vector<int> > routes)
{
    list->clear();
    DbManager dbm;

    for (auto route : routes) {
        std::string s{""};
        for (size_t i{0}; i < route.size() - 1; i++) {
            s += dbm.getIataForID(route[i]) + " -> ";
        }
        s += dbm.getIataForID(route[route.size() - 1]);
        list->addItem(QString{s.c_str()});
    }
}
