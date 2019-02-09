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
private:
    DbManager *_db ;
    std::vector<std::vector<u_int>> _adj;
    void create_adjacency_list();
    void BFS(int start);
    std::list<std::list<u_int> > get_all_routes(u_int start, u_int end);
};

#endif // ROUTEPLANNER_H
