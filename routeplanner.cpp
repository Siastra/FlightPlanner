#include "routeplanner.h"
#include <iostream>
#include <list>

RoutePlanner::RoutePlanner(DbManager *db)
{
    _db = db;

    std::cout << "Creating adj matrix, this might take a while..." << std::endl;
    create_adjacency_list();
    std::cout << "Done!" << std::endl;
}

void RoutePlanner::create_adjacency_list()
{
    u_int num_airport{_db->getAirportCount()};
    _adj.resize(num_airport);

    auto routes = _db->getAllRoutes();

    foreach (auto route, routes) {
        int airport1{route[1].toInt()};
        int airport2{route[2].toInt()};

        _adj[airport1].push_back(airport2);
    }

    BFS(69);
}

/* Breadth-first search */
void RoutePlanner::BFS(int start)
{
    u_int num_airport{_db->getAirportCount()};
    bool visited[num_airport];

    for (u_int i = 0; i < num_airport; ++i) {
        visited[i] = false;
    }

    std::list<u_int> queue;

    visited[start] = true;
    queue.push_back(start);

    while(!queue.empty())
    {
        int s = queue.front();
        std::cout << s << " ";
        queue.pop_front();

        for (u_int i{0}; i < _adj[s].size(); ++i)
        {
            u_int id{_adj[s][i]};

            if (!visited[id])
            {
                visited[id] = true;
                queue.push_back(id);
            }
        }
    }
}
