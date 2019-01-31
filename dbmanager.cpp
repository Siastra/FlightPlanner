#include "dbmanager.h"

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
       result.push_back({query.value(airlineC).toString(), getAirportName(query.value(airport1C).toString()), getAirportName(query.value(airport2C).toString())});
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
