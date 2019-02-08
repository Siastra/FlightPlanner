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
    std::vector<std::vector<bool>> _adjmx;

    void create_adjacency_matrix();
};

#endif // ROUTEPLANNER_H
