#include "routeplanner_astar.h"

Routeplanner_astar::Routeplanner_astar(DbManager *db) {
    this->db = db;
    this->get_connected_airports();
    this->get_positions_airports();
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

void Routeplanner_astar::get_positions_airports() {
    this->pos_airports = this->db->getLatLongOfAllAirports_double();
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
    auto airport1_pos = this->pos_airports.at(airport1-1);
    auto airport2_pos = this->pos_airports.at(airport2-1);
    return calcCrow(std::get<1>(airport1_pos), std::get<2>(airport1_pos), std::get<1>(airport2_pos), std::get<2>(airport2_pos));
}

std::vector<std::vector<int>> Routeplanner_astar::get_routes(int from, int to) {
    std::vector<std::vector<int>> fastest = this->get_routes_rec(from, to, 1, -1, 0);
    if (fastest.size() != 0) {
        std::vector<std::vector<int>> routes_unsorted = this->get_routes_rec(from, to, 1, fastest.at(0).size(), 0);
        routes_unsorted.push_back(fastest.at(0));
        std::sort( routes_unsorted.begin(), routes_unsorted.end() );
        routes_unsorted.erase( std::unique( routes_unsorted.begin(), routes_unsorted.end() ), routes_unsorted.end() );

        return routes_unsorted;
    } else {
        return fastest;
    }
}

std::vector<std::vector<int>> Routeplanner_astar::get_routes_rec(int from, int to, int rec_layer, int fastest_route, int steps_before) {
    std::vector<std::vector<int>> routes;

    PriorityQueue<int, double> frontier;
    frontier.put(from, 0);
    std::unordered_map<int, int> came_from;
    std::unordered_map<int, double> cost_so_far;
    came_from[from] = from;
    cost_so_far[from] = 0;

    int step = steps_before + 1;
    if (step > fastest_route && fastest_route != -1) {
      return routes;
    }

    while (!frontier.empty()) {
      int current = frontier.get();

      if (current == to) {
        break;
      } else if (conn_airpots[current].size() == 0) {
          return routes;
      }

      for (int next : conn_airpots[current]) {
        double new_cost = cost_so_far[current] + get_distance(current, next);

        if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
          cost_so_far[next] = new_cost;
          double priority = new_cost + get_distance(next, to);
          frontier.put(next, priority);
          came_from[next] = current;
        } else if (rec_layer <= 3 && fastest_route != -1) { // TODO check if airport is in came_from
            std::vector<std::vector<int>> new_routes = this->get_routes_rec(next, to, rec_layer + 1, fastest_route, step+1);
            std::vector<std::vector<int>> new_routes_fixed;

            std::vector<int> tmp_path;
            int current_tmp = next;
            bool flag = true;
            while (current_tmp != from) {
                if (flag) {
                    flag = false;
                } else {
                    tmp_path.push_back(current_tmp);
                }
              current_tmp = came_from[current_tmp];
            }

            tmp_path.push_back(from);
            std::reverse(tmp_path.begin(), tmp_path.end());

            foreach (auto route_tmp, new_routes) {
                std::vector<int> tmp(tmp_path);
                tmp.insert(tmp.end(), route_tmp.begin(), route_tmp.end());
                new_routes_fixed.push_back(tmp);
            }

            foreach (auto route, new_routes_fixed) {
                routes.push_back(route);
            }
        }
        step = steps_before;
        int tmp = current;
        while (tmp != from) {
          tmp = came_from[tmp];
          step += 1;
        }
        if (step > fastest_route - 1 && fastest_route != -1) {
          return routes;
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