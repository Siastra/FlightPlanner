/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#ifndef AIRPORT_H
#define AIRPORT_H

#include <QString>

class Airport
{
public:
  int id;
  double lat;
  double lon;
  QString name;
  QString iata;

  Airport(int _id, double _lat, double _lon, QString _name, QString _iata)
  {
    id = _id;
    lat = _lat;
    lon = _lon;
    name = _name;
    iata = _iata;
  }

  Airport() {}
};

#endif // AIRPORT_H
