#include "routeplanner.h"
#include <iostream>

RoutePlanner::RoutePlanner(DbManager *db)
{
    _db = db;

    std::cout << "Creating adj matrix, this might take a while..." << std::endl;
    create_adjacency_matrix();
    std::cout << "Done!" << std::endl;

}

void RoutePlanner::create_adjacency_matrix()
{
    u_int num_airport{_db->getAirportCount()};

    _adjmx.resize(num_airport);

    for (u_int i{0}; i < num_airport; ++i) {
        _adjmx[i].resize(num_airport);
    }

    auto routes = _db->getAllRoutes();

    for (u_int i{0}; i < routes.size(); ++i) {
        auto route{routes[i]};
        QString airport1{route[1]};
        auto airport2{route[2]};

        std::cout << airport1.toStdString() << std::endl;
    }
}
