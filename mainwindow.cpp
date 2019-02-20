/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    abd.setWindowTitle("FlightPlanner - About");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    try {
        if (ui->FromSearch->text().toStdString() == "" && ui->ToSearch->text().toStdString() == "") {
            return;
        }
        auto from_id{dbm.getAirportIdFromInput(ui->FromSearch->text().toStdString())};
        auto to_id{dbm.getAirportIdFromInput(ui->ToSearch->text().toStdString())};

        // std::cout << rpl.get_min_hops(from_id, to_id) << std::endl;

        // A STAR ALGO
        clock_t begin = clock();
        //auto routes = rpl.get_routes(from_id, to_id);  // UNCOMMENT FOR TESTING SECOND ALGO
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        std::cout << "get_routes astar: " << elapsed_secs << std::endl;

        // BREADTH FIRST SEARCH ALGO
        begin = clock();
        auto routes = rpl.get_routes_hops(from_id, to_id);
        end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        // routes = routes_hops; // TODO COMMENT
        std::cout << "get_routes hops: " << elapsed_secs << std::endl;

        //auto routes = rpl.get_routes(from_id, to_id);
        size_t min = 1000;
        for (auto route : routes) {
            if (route.size() < min) {
                min = route.size();
            }
        }
        ui->map->connectTheDots(routes);
        fillTable(ui->flighttable, routes);
    } catch (...) {}
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

void MainWindow::on_flighttable_itemClicked(QListWidgetItem *item)
{
    if (selItem == item) {
        on_pushButton_clicked();
    }else{
        std::vector<int> route;
        DbManager dbm{};
        auto parts = item->text().split("->");
        QRegularExpression re("(?<iata>\\w+).*");
        for (auto part : parts) {
            auto match = re.match(part.simplified());
            if (match.hasMatch()) {
                route.push_back(dbm.getAirportIDForIATA(match.captured("iata")));
            }
        }

        std::vector<std::vector<int>> tmp;
        tmp.push_back(route);
        ui->map->connectTheDots(tmp);
        selItem = item;
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    abd.show();
}
