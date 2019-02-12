#include "routeplanner_astar.h"

Routeplanner_astar::Routeplanner_astar(DbManager *db) {
    this->db = db;
    this->get_connected_airports();

}

void Routeplanner_astar::init(DrawableMapWidget *dw, QTableView* flighttable){
    this->_dw = dw;
    this->_flighttable = flighttable;
}

void Routeplanner_astar::get_connected_airports() {
    int airports{this->db->getAirportCount()};
    this->conn_airpots.resize(airports);
    auto routes = this->db->getAllRoutes();

    foreach (auto route, routes) {
        int airport1{route[1].toInt()};
        int airport2{route[2].toInt()};
        if (std::find(this->conn_airpots[airport1].begin(), this->conn_airpots[airport1].end(), airport2) == this->conn_airpots[airport1].end()) {
          this->conn_airpots[airport1].push_back(airport2);
        }
    }
}

int Routeplanner_astar::get_min_hops(int from, int to) {
    auto routes = this->get_routes(from, to);
    if (routes.size()) {
        return routes.at(0).size();
    } else {
        return 0;
    }
}

double Routeplanner_astar::get_distance(int airport1, int airport2) {
    auto airport1_pos = this->db->getLatLongOfAirport(airport1);
    auto airport2_pos = this->db->getLatLongOfAirport(airport2);
    return calcCrow(std::get<1>(airport1_pos), std::get<2>(airport1_pos), std::get<1>(airport2_pos), std::get<2>(airport2_pos));
}

Airport_node Routeplanner_astar::create_node(int id, Airport_node *parent_) {
    auto pos = this->db->getLatLongOfAirport(id);
    Airport_node ret = Airport_node(id, std::get<1>(pos), std::get<2>(pos), parent_);
    return ret;
}

Airport_node* Routeplanner_astar::findNodeOnList(std::vector<Airport_node*>& nodes_, int id) {
    for (auto node : nodes_) {
        if (node->id == id) {
            return node;
        }
    }
    return nullptr;
}

std::vector<std::vector<int>> Routeplanner_astar::get_routes(int from, int to) {
    std::vector<std::vector<int>> routes;
    std::cout << "in get routes" << std::endl;
    //std::cout << from << std::endl;
    //std::cout << to << std::endl;

    double d = this->get_distance(from, to);
    std::cout << "distance: " << d << std::endl;

    std::vector<int> airports = conn_airpots[from];
    foreach (int airport_id, airports) {
        std::cout << airport_id << std::endl;
    }

    PriorityQueue<int, double> frontier;
    frontier.put(from, 0);
    std::unordered_map<int, int> came_from;
    std::unordered_map<int, double> cost_so_far;

    came_from[from] = from;
    cost_so_far[from] = 0;

    while (!frontier.empty()) {
      int current = frontier.get();

      if (current == to) {
        break;
      }

      for (int next : conn_airpots[current]) {
        double new_cost = cost_so_far[current] + get_distance(current, next);

        if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
          cost_so_far[next] = new_cost;
          double priority = new_cost + get_distance(next, to);
          frontier.put(next, priority);
          came_from[next] = current;
        }
      }
    }

    std::vector<int> path;
    int current = to;
    while (current != from) {
      path.push_back(current);
      current = came_from[current];
    }
    path.push_back(from);
    std::reverse(path.begin(), path.end());
    routes.push_back(path);
    foreach (auto airport, path) {
        std::cout << airport << std::endl;
    }
    /*
    Airport_node *current = nullptr;
    std::vector<Airport_node*> openSet, closedSet;
    Airport_node first = this->create_node(from);
    openSet.push_back(&first);
    openSet.reserve(100);
    closedSet.reserve(100);

    while (!openSet.empty()) {
        auto current_it = openSet.begin();
        current = *current_it;

        for (auto it = openSet.begin(); it != openSet.end(); it++) {
            auto node = *it;
            if (node->getScore() <= current->getScore()) {
                current = node;
                current_it = it;
            }
        }

        if (current->id == to) {
            break;
        } else {
            //std::cout << current->id << std::endl;
        }

        closedSet.push_back(current);
        openSet.erase(current_it);

        auto conn_to_curr = conn_airpots[current->id];
        //foreach (auto airport__, conn_to_curr) {
        //    std::cout << airport__ << std::endl;
        //}
        //return routes;

        foreach (auto next_id, conn_to_curr) {
            if (findNodeOnList(closedSet, next_id)) {
                std::cout << next_id << std::endl;
                continue;
            }

            double totalCost = current->G + get_distance(current->id, next_id);

            Airport_node *successor = findNodeOnList(openSet, next_id);
            if (successor == nullptr) {
                Airport_node tmp = this->create_node(next_id, current);
                successor = &tmp;
                successor->G = totalCost;
                successor->H = get_distance(next_id, to);
                //std::cout << "next id: " << next_id << std::endl;
                //std::cout << "H: " << successor->H << std::endl;
                //std::cout << "OpenSet: " << openSet.size() << std::endl;
                //std::cout << "ClosedSet: " << closedSet.size() << std::endl;
                openSet.push_back(successor);
            }
            else if (totalCost < successor->G) {
                successor->parent = current;
                successor->G = totalCost;
            }
        }
    }

    std::vector<int> path;
    while (current != nullptr) {
        path.push_back(current->id);
        current = current->parent;
    }

    //releaseNodes(openSet);
    //releaseNodes(closedSet);

    routes.push_back(path);
    foreach (auto airport, path) {
        std::cout << airport << std::endl;
    }*/

    return routes;
}

double Routeplanner_astar::toRad(double value) {
  return value * M_PI / 180;
}

double Routeplanner_astar::calcCrow(double lat1, double lon1, double lat2, double lon2) {
  int R = 6371;
  double dLat = toRad(lat2 - lat1);
  double dLon = toRad(lon2 - lon1);
  lat1 = toRad(lat1);
  lat2 = toRad(lat2);

  double a = sin(dLat / 2) * sin(dLat / 2) +
    sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double d = R * c;
  return d;
}
