/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#include "dbmanager.h"
#include <QRegularExpression>
#include <thread>

DbManager::DbManager()
{
}

DbManager::DbManager(const QString& path)
{
   DbManager::routes = std::vector<Route>();
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
   loadAirports();
   loadAirlines();
   loadRoutes();
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
    return airports.at(id.toInt()).name;
}

QStringList DbManager::getAllAirportNames() {
    QStringList result;
    QSqlQuery query("SELECT name, iata FROM Airport;");
    int name = query.record().indexOf("name");
    int iata = query.record().indexOf("iata");
    while ( query.next() ) {
        result.push_back(QString(query.value(name).toString().simplified().trimmed() + " (" +query.value(iata).toString() + ")"));
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

std::vector<std::tuple<int, double, double>> DbManager::getLatLongOfAllAirports_double() {
    std::vector<std::tuple<int, double, double>> ret;
    ret.reserve(5611);
    QSqlQuery query("SELECT id, latitude, longitude from Airport order by id asc;");
    int id = query.record().indexOf("id");
    int latitude = query.record().indexOf("latitude");
    int longitude = query.record().indexOf("longitude");

    while (query.next()) {
        ret.push_back(std::make_tuple<int, double, double>(query.value(id).toInt(), query.value(latitude).toDouble(), query.value(longitude).toDouble()));
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
    QRegularExpression re(".*\\((?<iata>\\w+)\\).*");
    auto match = re.match(input.c_str());

    if (match.hasMatch()) {

        QSqlQuery query("SELECT id FROM Airport where iata = \"" + match.captured("iata") + "\";");
        query.next();
        int col{query.record().indexOf("id")};

        std::cout << match.captured("iata").toStdString() << " "  << query.value(col).toInt() << std::endl;

        return query.value(col).toInt();
    }
    return -1;
}

int DbManager::getAirportIDForIATA(QString iata)
{
    QSqlQuery query;
    query.prepare("select id from Airport where iata = :iata");
    query.bindValue(":iata", iata);
    query.exec();

    auto id = query.record().indexOf("id");

    query.next();
    return query.value(id).toInt();
}

std::string DbManager::getIataForID(int id)
{
    return airports.at(id).iata.toStdString();
}

Airline DbManager::getAirlineForID(int id)
{
    return airlines.at(id);
}

std::string DbManager::getAirlineForRoute(int a1, int a2)
{
    QSqlQuery query;
    query.prepare("select trim(a.name) \"name\", a.alliance from Route r \
                     join Airline a on r.airline = a.id \
                    where airport1 = :a1 and airport2 = :a2;");
    query.bindValue(":a1", a1);
    query.bindValue(":a2", a2);

    query.exec();
    query.next();

    return query.value("name").toString().simplified().trimmed().toStdString();

}

int DbManager::getAllianceForAirline(QString airline)
{
    QSqlQuery query;
    query.prepare("select coalesce(alliance, -1) from Airline where trim(name) = :airline");
    query.bindValue(":airline", airline);

    query.exec();
    query.next();
    query.first();

    return query.value("coalesce(alliance, -1)").toInt();
}

int DbManager::getAirportCount()
{
    QSqlQuery query("SELECT count(*) FROM Airport;");
    query.next();
    int col{query.record().indexOf("count(*)")};

    return query.value(col).toInt();
}

int DbManager::getAirlineCount()
{
    QSqlQuery query("SELECT count(*) FROM Airline;");
    query.next();
    int col{query.record().indexOf("count(*)")};

    return query.value(col).toInt();
}

int DbManager::getRouteCount()
{
    QSqlQuery query("SELECT count(*) FROM Route;");
    query.next();
    int col{query.record().indexOf("count(*)")};

    return query.value(col).toInt();
}

Airport* DbManager::getAirport(QVariant id) {
    return &airports.at(id.toInt());
}

std::string DbManager::getNameForID(int id) {
    return airports.at(id).name.toStdString();
}

void DbManager::loadAirports()
{
    airports.resize(getAirportCount() + 1);
    QSqlQuery query;
    query.prepare("select * from airport");
    query.exec();

    auto idCol = query.record().indexOf("id");
    auto latitudeCol = query.record().indexOf("latitude");
    auto longitudeCol = query.record().indexOf("longitude");
    auto nameCol = query.record().indexOf("name");
    auto iataCol = query.record().indexOf("iata");

    while (query.next())
    {
        auto id{query.value(idCol).toInt()};
        airports.at(id) = Airport(id,
                                  query.value(latitudeCol).toDouble(), query.value(longitudeCol).toDouble(),
                                  query.value(nameCol).toString().simplified().trimmed(), query.value(iataCol).toString());
    }
}

void DbManager::loadRoutes() {
    DbManager::routes.resize(getRouteCount() + 1);
    QSqlQuery query;
    query.prepare("select * from Route");
    query.exec();

    auto airlineCol = query.record().indexOf("airline");
    auto airport1Col = query.record().indexOf("airport1");
    auto airport2Col = query.record().indexOf("airport2");

    size_t i = 0;
    while (query.next())
    {
        DbManager::routes.at(i) = Route(getAirport(query.value(airport1Col)), getAirport(query.value(airport2Col)), getAirlineForID(query.value(airlineCol).toInt()));
        i++;
    }
}

void DbManager::loadAirlines() {
    DbManager::airlines.resize(getAirlineCount() + 1);
    QSqlQuery query;
    query.prepare("select * from airline");
    query.exec();

    auto idCol = query.record().indexOf("id");
    auto nameCol = query.record().indexOf("name");
    auto allianceCol = query.record().indexOf("alliance");

    while (query.next())
    {
        auto id{query.value(idCol).toInt()};
        airlines.at(id) = Airline(id,
                                  query.value(nameCol).toString(), query.value(allianceCol).toInt());
    }
}
