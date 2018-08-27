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

 graph(unsigned int, double, double, double);
  
 ~graph();
 
 graph(graph &);

 std::vector<std::vector<double> > random_edges();
 
 void delete_all_edges();

 int num_non_zero_edges();

 int num_vertices();
 
 bool adjacent(int, int);

 void add_edge(int, int, double);

 void delete_edge(int, int);

 double get_edge_value(int, int);

 std::vector<int> my_neighbors(int);

 bool is_connected();

 std::vector<int> find_path(int, int);


 
 private:
 
 unsigned int n_nodes;
 double density, low_edge_value, high_edge_value;
 std::vector<std::vector<double> > edges;

};

#endif
