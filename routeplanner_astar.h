/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#ifndef ROUTEPLANNER_ASTAR_H
#define ROUTEPLANNER_ASTAR_H

#include <vector>
#include <QTableView>
#include <math.h>
#include <queue>
#include <unordered_map>
#include <tuple>
#include <set>
#include <future>
#include <algorithm>
#include "dbmanager.h"
#include "drawablemapwidget.h"

#include <iostream> // TODO: remove later

template<typename T, typename priority_t>
struct PriorityQueue {
  typedef std::pair<priority_t, T> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>,
                 std::greater<PQElement>> elements;

  inline bool empty() const {
     return elements.empty();
  }

  inline void put(T item, priority_t priority) {
    elements.emplace(priority, item);
  }

  T get() {
    T best_item = elements.top().second;
    return best_item;
  }

  void pop() {
      elements.pop();
  }
};

class Routeplanner_astar {
public:
    Routeplanner_astar(DbManager *db);
    std::vector<std::vector<int>> get_routes(int from, int to, bool &running);
    std::vector<std::vector<int>> get_routes_hops(int from, int to, bool &running);
    std::vector<std::vector<int>> sort_routes_distance(std::vector<std::vector<int>> routes);
    int getShortestDepth_fast(int from, int to);
    std::vector<std::vector<int>> getRoutes_fast(int from, int to);
    std::vector<int> get_fastest_route_astar(int form, int to);
    double get_route_distance(std::vector<int> route);
    int get_min_hops(int from, int to);
private:
    DbManager *db = new DbManager();
    QTableView* _flighttable;
    std::vector<std::vector<int>> get_routes_rec(int from, int to, int rec_layer, int fastest_route, int steps_before, bool &running);
    std::vector<std::vector<int>> get_routes_hops_rec(std::vector<int> prev, int depth, int from, int to, bool &running);
    bool is_connected(int from, int to);
    void get_connected_airports();
    void get_positions_airports();
    double get_distance(int airport1, int airport2);

    std::vector<std::tuple<int, double, double>> pos_airports;
    std::vector<std::vector<int>> conn_airpots;
    double toRad(double value);
    double calcCrow(double lat1, double lon1, double lat2, double lon2);
};

#endif // ROUTEPLANNER_ASTAR_H
