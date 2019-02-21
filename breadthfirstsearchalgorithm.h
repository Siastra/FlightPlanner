#ifndef BREADTHFIRSTSEARCHALGORITHM_H
#define BREADTHFIRSTSEARCHALGORITHM_H

#include <map>
#include <vector>
#include <queue>
#include "dbmanager.h"

class BreadthFirstSearchAlgorithm {
  public:
    DbManager *db = new DbManager();
    std::vector<std::tuple<int, double, double>> pos_airports;
    std::vector<std::vector<int>> conn_airpots;
    BreadthFirstSearchAlgorithm(DbManager *db) {
        this->db = db;
        this->get_connected_airports();
        this->get_positions_airports();
    }
    std::vector<std::vector<int>> getRoutes(int start, int end) {
        std::vector<std::vector<int>> routes;
        int depth = getShortestDepth(start, end);

        auto predecessors = generatePredecessors(start, depth);
        routes = backtrack({end}, depth, end, start, predecessors);

        for (auto &route : routes) {
            std::reverse(route.begin(), route.end());
        }

        return routes;
    }

  private:
    void get_connected_airports() {
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

    void get_positions_airports() {
        this->pos_airports = this->db->getLatLongOfAllAirports_double();
    }

    std::vector<std::vector<int>> backtrack(std::vector<int> prev, int depth, int start, int end, std::map<int, std::vector<int>> &predecessors) {
        std::vector<std::vector<int>> ret;

        if (depth == 0) {
            if (start == end) {
                std::vector<int> newPrev = prev;
                //newPrev.push_back(end);
                ret.push_back(newPrev);
            }
        }
        else {
            auto nbs = predecessors[start];
            for (auto &airport : nbs) {
                if (std::find(prev.begin(), prev.end(), airport) != prev.end()) {
                    continue;
                }

                std::vector<int> newPrev = prev;
                newPrev.push_back(airport);

                auto toConcat = backtrack(newPrev, depth - 1, airport, end, predecessors);
                if (toConcat.size() != 0)
                {
                    ret.insert(ret.end(), toConcat.begin(), toConcat.end());
                }
            }
        }
        return ret;
    }

    std::map<int, std::vector<int>> generatePredecessors(int start, int goalDepth) {
        int depth{0};
        std::map<int, std::vector<int>> result;
        std::queue<int> currentLayer;
        std::queue<int> nextLayer;
        std::map<int, bool> visited;

        currentLayer.push(start);
        int next = start;

        while (depth != goalDepth)
        {
            if (!visited[next])
            {
                visited[next] = true;
                for (auto &airport : this->conn_airpots[next]) {
                    if (visited[airport] != true) {
                        nextLayer.push(airport);
                        result[airport].push_back(next);
                    }
                }
            }

            currentLayer.pop();
            if (currentLayer.empty())
            {
                depth += 1;
                currentLayer = nextLayer;
                if (currentLayer.empty())
                {
                    return result;
                }
            }
            next = currentLayer.front();
        }

        return result;
    }

    int getShortestDepth(int start, int end) {
        int depth{0};
        std::queue<int> currentLayer;
        std::queue<int> nextLayer;
        std::map<int, bool> visited;

        currentLayer.push(start);
        visited[start] = true;
        int next = start;
        while (next != end) {
            for (auto &airport : this->conn_airpots[next]) {
                if (visited[airport] != true)
                {
                    visited[airport] = true;
                    nextLayer.push(airport);
                }
            }

            currentLayer.pop();
            if (currentLayer.empty()) {
                depth += 1;
                currentLayer = nextLayer;
                if (currentLayer.empty()) {
                    return -1;
                }
            }
            next = currentLayer.front();
        }
        return depth;
    }
};

#endif // BREADTHFIRSTSEARCHALGORITHM_H
