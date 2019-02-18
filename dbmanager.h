/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtDebug>
#include <vector>
#include <tuple>
#include <QStringList>
#include <string>
#include "airport.h"

class DbManager
{
public:
    DbManager();
    DbManager(const QString& path);
    std::vector<std::vector<QString>> getAllRoutes();
    QString getAirportName(QString id);
    QStringList getAllAirlineNames();
    QStringList getAllAirportNames();
    std::tuple<int, double, double> getLatLongOfAirport(int airport_id);
    std::vector<std::tuple<int, double, double>> getLatLongOfAllAirports_double();
    QList<std::tuple<int, int, int>> getLatLongOfAllAirports();
    int getAirportCount();
    int getAirportIdFromInput(std::string input);
    int getAirportIDForIATA(QString iata);

    std::string getIataForID(int id);
    std::string getNameForID(int id);

    std::vector<Airport> airports;
    void loadAirports();

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
