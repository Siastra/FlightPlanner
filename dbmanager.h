#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtDebug>
#include <vector>
#include <QStringList>

class DbManager
{
public:
    DbManager(const QString& path);
    std::vector<std::vector<QString>> getAllRoutes();
    QString getAirportName(QString id);
    QStringList getAllAirlineNames();
    QStringList getAllAirportNames();
private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
