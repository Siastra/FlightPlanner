#include "routeplanner.h"
#include <iostream>
#include <list>
#include <stack>

RoutePlanner::RoutePlanner(DbManager *db)
{
    _db = db;

    create_adjacency_list();
    auto routes_to_end = get_all_routes(4908, 48);
    print_list_list(routes_to_end);
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

/* Depth-first search */
std::list<std::list<u_int>> RoutePlanner::get_all_routes(u_int start, u_int end)
{
    std::list<std::stack<u_int>> results;
    u_int num_airport{_db->getAirportCount()};
    std::vector<bool> visited(num_airport, false);

    std::stack<u_int> stack;
    stack.push(start);

    while (!stack.empty())
    {
        u_int curr_node{stack.top()};

        foreach (u_int node, _adj[curr_node]) {
            if (!visited[node] && node != end) {
                stack.push(node);
                visited[node] = true;
                continue;
            } else if (node == end && !visited[node])
            {
                visited[node] = true;
                stack.push(end);
                results.push_back(stack);
                stack.pop();
            }
        }

        if (curr_node == stack.top()) {
            visited[end] = false;
            stack.pop();
        }
    }

    std::list<std::list<u_int>> formatted_results;

    foreach (auto route, results)
    {
        std::list<u_int> new_route;

        while (!route.empty()) {
            new_route.push_front(route.top());
            route.pop();
        }

        formatted_results.push_front(new_route);
    }

    return formatted_results;
}
