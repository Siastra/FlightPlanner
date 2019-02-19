/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#ifndef ROUTE_H
#define ROUTE_H

#include "airline.h"
#include "airport.h"

class Route
{
public:
    Route(Airport _airportFrom, Airport _airportTo, Airline _airline) {
        airportFrom = _airportFrom;
        airportTo = _airportTo;
        airline = _airline;
    }
    Route() {}
    Airline airline;
    Airport airportFrom;
    Airport airportTo;
};

#endif // ROUTE_H
