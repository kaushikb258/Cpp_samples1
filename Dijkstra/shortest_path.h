#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "graph.h"
#include "queue.h"


class short_path
{

 public:

 short_path(int, double, double, double);

 ~short_path();

 short_path(short_path&);

 void check_if_graph_connected();

 std::vector<int> list_of_vertices(int, int);

 double path_length(int, int);


 private:

 unsigned int n_nodes;
 double density, low_edge_value, high_edge_value;
 graph *g;

};

#endif
