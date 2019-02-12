#ifndef ROUTEPLANNER_ASTAR_H
#define ROUTEPLANNER_ASTAR_H

#include <vector>
#include <QTableView>
#include <math.h>
#include <queue>
#include <unordered_map>
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
    elements.pop();
    return best_item;
  }
};

class Routeplanner_astar {
public:
    Routeplanner_astar(DbManager *db);
    void init(DrawableMapWidget* dw, QTableView* flighttable);
    std::vector<std::vector<int>> get_routes(int from, int to);
    int get_min_hops(int from, int to);
private:
    DbManager *db;
    DrawableMapWidget* _dw;
    QTableView* _flighttable;
    void get_connected_airports();
    double get_distance(int airport1, int airport2);
    std::vector<std::vector<int>> conn_airpots;
    double toRad(double value);
    double calcCrow(double lat1, double lon1, double lat2, double lon2);
};

#endif // ROUTEPLANNER_ASTAR_H
