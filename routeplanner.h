#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include <vector>
#include <stack>
#include "dbmanager.h"

class RoutePlanner
{   
public:
    RoutePlanner(DbManager *db);
    void print_list_list(std::list<std::list<u_int> > list_list);
    std::list<std::list<u_int> > get_routes(u_int start, u_int end);
    u_int get_min_hops(u_int from, u_int to);

private:
    DbManager *_db ;
    std::vector<std::vector<u_int>> _adj;
    void create_adjacency_list();
};

#endif // ROUTEPLANNER_H
