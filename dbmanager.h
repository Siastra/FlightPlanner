#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtDebug>
#include <vector>
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

    QList<std::tuple<int, int, int>> getLatLongOfAllAirports();
    u_int getAirportCount();
    u_int getAirportIdFromInput(std::string input);
private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
