#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QCompleter>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
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
