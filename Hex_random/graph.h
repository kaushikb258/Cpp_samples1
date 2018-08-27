#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "queue.h"


class graph
{

 public:

 graph();

 graph(int); 

 ~graph();
 
 graph(graph &);

 void delete_all_edges();

 bool adjacent(int, int);
 
 void add_node(int);

 void add_edge(int, int);

 std::vector<int> my_neighbors(int);

 bool is_connected();

 int get_index(int);
 
 std::vector<int> find_path(int, int);
 
 std::vector<int> get_nodes(); 

 private:
 
 unsigned int n_nodes;
 std::vector<int> nodes;
 std::vector<std::vector<int> > edges;

};

#endif
