#include "routeplanner.h"
#include <iostream>
#include <list>
#include <stack>

RoutePlanner::RoutePlanner(DbManager *db)
{
    _db = db;

    create_adjacency_list();
    std::cout << "min: " << get_min_hops(4908, 1088) << std::endl;
}

void RoutePlanner::init(DrawableMapWidget *dw, QTableView* flighttable)
{
    this->_dw = dw;
    this->_flighttable = flighttable;
}

void RoutePlanner::print_list_list(std::list<std::list<u_int>> list_list) {
    foreach (auto list, list_list)
    {
        foreach (auto entry, list)
        {
            std::cout << entry << " ";
        }

        std::cout << "####" << std::endl;
    }
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
}

/* Breath-first search, seperator at the end of each row to keep count */
u_int RoutePlanner::get_min_hops(u_int from, u_int to)
{
    u_int num_airport{_db->getAirportCount()};
    std::vector<bool> visited(num_airport, false);
    visited[from] = true;

    std::list<u_int> queue;
    queue.push_back(from);
    u_int layer{0};
    u_int layer_seperator{from};

    while(!queue.empty())
    {
        auto curr_node = queue.front();
        queue.pop_front();
        bool new_layer{curr_node == layer_seperator};

        foreach (auto node, _adj[curr_node])
        {

            if (!visited[node])
            {

                if (new_layer) {
                    layer_seperator = node;
                    new_layer = false;
                    layer++;
                }

                if (node == to) {
                    return layer;
                }

                visited[node] = true;
                queue.push_back(node);
            }
        }
    }

    return layer;
}
