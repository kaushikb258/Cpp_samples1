#ifndef MIN_SPANNING_H
#define MIN_SPANNING_H

#include <iostream>
#include <vector>

#include "graph.h"
#include "queue.h"
#include "quick_sort.h"


class min_spanning_tree
{

 public:

 min_spanning_tree(int, double, double, double);

 ~min_spanning_tree();

 min_spanning_tree(min_spanning_tree&);
 
 void kruskal_mst();

 bool check_edge(std::vector<std::vector<int> > &, int, int);
 
 private:

 unsigned int n_nodes;
 double density, low_edge_value, high_edge_value;
 graph *g;

};

#endif
