#include <vector>
#include <iostream>

#include "dbmanager.h"

#include <QFile>

DbManager::DbManager()
{

}

DbManager::DbManager(const QString& path)
{
   m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(path);
   qDebug() << path;

   if (!m_db.open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}

std::vector<std::vector<QString>> DbManager::getAllRoutes() {
    std::vector<std::vector<QString>> result;
    QSqlQuery query("SELECT * FROM Route;");
    int airlineC = query.record().indexOf("airline");
    int airport1C = query.record().indexOf("airport1");
    int airport2C = query.record().indexOf("airport2");
    while (query.next())
    {
       result.push_back({query.value(airlineC).toString(), query.value(airport1C).toString(), query.value(airport2C).toString()});
    }
    return result;
}

QStringList DbManager::getAllAirlineNames() {
    QStringList result;
    QSqlQuery query("SELECT name FROM Airline;");
    int nameC = query.record().indexOf("name");
    while (query.next())
    {
       result.push_back(query.value(nameC).toString());
    }
    return result;
}

QString DbManager::getAirportName(QString id){
    std::vector<QString> result;
    QSqlQuery query("SELECT name FROM Airport WHERE id = " + id + ";");
    int nameC = query.record().indexOf("name");
    query.next();
    return query.value(nameC).toString().simplified();
}

QStringList DbManager::getAllAirportNames() {
    QStringList result;
    QSqlQuery query("SELECT name, iata FROM Airport;");
    int name = query.record().indexOf("name");
    int iata = query.record().indexOf("iata");
    while ( query.next() ) {
        result.push_back(QString(query.value(name).toString().simplified() + "(" +query.value(iata).toString() + ")"));
    }
    return result;
}

QList<std::tuple<int, int, int>> DbManager::getLatLongOfAllAirports()
{
    QList<std::tuple<int, int, int>> ret;
    QSqlQuery query("SELECT id, latitude, longitude from Airport;");

    while (query.next()) {
        ret.push_back(std::make_tuple<int, int, int>(query.value("id").toInt(), query.value("latitude").toInt(), query.value("longitude").toInt()));  // Lat und Long zu int???
    }
    return ret;
}

std::tuple<int, double, double> DbManager::getLatLongOfAirport(int airport_id) {
    QSqlQuery query(QString::fromStdString("SELECT id, latitude, longitude from Airport where id = " + std::to_string(airport_id) + ";"));
    int id = query.record().indexOf("id");
    int latitude = query.record().indexOf("latitude");
    int longitude = query.record().indexOf("longitude");

    QList<std::tuple<int, double, double>> ret;
    while (query.next()) {
        ret.push_back(std::make_tuple<int, double, double>(query.value(id).toInt(), query.value(latitude).toDouble(), query.value(longitude).toDouble()));
    }

    return ret.at(0);
}

int DbManager::getAirportIdFromInput(std::string input)
{
    auto iata{input.substr(input.size() - 4, 3)};

    QSqlQuery query("SELECT id FROM Airport where iata = \"" + QString::fromStdString(iata) + "\";");
    query.next();
    int col{query.record().indexOf("id")};

    std::cout << iata << " "  << query.value(col).toInt() << std::endl;

    return query.value(col).toInt();
}

std::string DbManager::getIataForID(int id)
{
    QSqlQuery query;
    query.prepare("select iata from Airport where id = :id");
    query.bindValue(":id", id);
    query.exec();

    query.next();
    return query.value(0).toString().toStdString();
}

int DbManager::getAirportCount()
{
    QSqlQuery query("SELECT count(*) FROM Airport;");
    query.next();
    int col{query.record().indexOf("count(*)")};

    return query.value(col).toInt();
}

