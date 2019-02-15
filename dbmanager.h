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

    std::string getIataForID(int id);
private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
