#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QCompleter>


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

    rpl.init(ui->map, ui->flighttable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    auto from_id{dbm.getAirportIdFromInput(ui->FromSearch->text().toStdString())};
    auto to_id{dbm.getAirportIdFromInput(ui->ToSearch->text().toStdString())};

    std::cout << rpl.get_min_hops(from_id, to_id) << std::endl;

    //auto routes{rpl.get_routes(from_id, to_id)};
    //rpl.print_list_list(routes);
}
