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

        std::vector<std::future<std::vector<std::vector<int>>>> routes_from_algo;
        bool running = true;

        // A STAR ALGO
        routes_from_algo.push_back(std::async(std::launch::async, [&]{
            clock_t begin = clock();
            auto routes = rpl.get_routes(from_id, to_id, running);  // UNCOMMENT FOR TESTING SECOND ALGO
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            std::cout << "get_routes astar: " << elapsed_secs << std::endl;
            return routes;
        }));

        // BREADTH FIRST SEARCH ALGO
        routes_from_algo.push_back(std::async(std::launch::async, [&]{
            clock_t begin = clock();
            auto routes = rpl.get_routes_hops(from_id, to_id, running);
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            std::cout << "get_routes hops: " << elapsed_secs << std::endl;
            return routes;
        }));

        // BREADTH FIRST SEARCH ALGO WITH BACKTRACKING
        routes_from_algo.push_back(std::async(std::launch::async, [&]{
            clock_t begin = clock();
            BreadthFirstSearchAlgorithm br(&dbm);
            auto routes = br.getRoutes(from_id, to_id);
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            std::cout << "get_routes hops: " << elapsed_secs << std::endl;
            return routes;
        }));

        std::vector<std::vector<int>> routes;
        while (running) {
            for (int i = 0; i < routes_from_algo.size(); i++) {
                if (routes_from_algo.at(i).wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    routes = routes_from_algo.at(i).get();
                    running = false;
                }
            }
        }

        /*
        {
            std::vector<int> tmp_vec = rpl.get_fastest_route_astar(from_id, to_id);
            if (tmp_vec.size() != 0) {
                routes.push_back(tmp_vec);
            }
        }*/

        std::cout << "got routes: " << routes.size() << std::endl;

        // SORT ROUTES
        clock_t begin = clock();
        routes = rpl.sort_routes_distance(routes);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        std::cout << "sorting " << routes.size() << " routes took: " << elapsed_secs << std::endl;

        routes.erase(std::unique( routes.begin(), routes.end()), routes.end());

        size_t min = 1000;
        for (auto route : routes) {
            if (route.size() < min) {
                min = route.size();
            }
        }
        if (routes.size() > 0) {
            ui->map->connectTheDots(routes, ui->airlineSearch->text());
            fillTable(ui->flighttable, routes);
        }else{
            ui->map->resetPic();
            ui->map->update();
            ui->flighttable->clear();
            ui->flighttable->addItem("No routes available!");
        }

        for (int i = 0; i < routes_from_algo.size(); i++) {
            routes_from_algo.at(i).get();
        }

    } catch (...) {}
}

void MainWindow::fillTable(QListWidget *list, std::vector<std::vector<int> > routes)
{
    list->clear();

    for (auto route : routes) {
        std::string s{""};
        for (size_t i{0}; i < route.size() - 1; i++) {
            s += dbm.getIataForID(route[i]) + " (" + dbm.getAirlineForRoute(route[i], route[i+1]) + ") -> ";
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
        auto parts = item->text().split("->");
        QRegularExpression re("(?<iata>\\w+).*");
        for (auto part : parts) {
            auto match = re.match(part.simplified().trimmed());
            if (match.hasMatch()) {
                route.push_back(dbm.getAirportIDForIATA(match.captured("iata")));
            }
        }

        std::vector<std::vector<int>> tmp;
        tmp.push_back(route);
        ui->map->connectTheDots(tmp, ui->airlineSearch->text());
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
