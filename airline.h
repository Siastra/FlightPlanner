/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#ifndef AIRLINE_H
#define AIRLINE_H

#include <QString>

class Airline
{
public:
    Airline() {}
    Airline(int _id, QString _name, int _alliance) {
        id = _id;
        name = _name;
        alliance = _alliance;
    }
    int id;
    QString name;
    int alliance;
};

#endif // AIRLINE_H
