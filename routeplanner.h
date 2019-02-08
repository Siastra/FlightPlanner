#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include <vector>

#include "dbmanager.h"

class RoutePlanner
{   
public:
    RoutePlanner(DbManager *db);
private:
    DbManager *_db ;
    std::vector<std::vector<u_int>> _adj;
    void create_adjacency_list();
    void BFS(int start);
};

#endif // ROUTEPLANNER_H
