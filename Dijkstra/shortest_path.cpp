#include <iostream>
#include <vector>

#include "graph.h"
#include "queue.h"
#include "shortest_path.h"

using namespace std;


// constructor
short_path::short_path(int n_nodes, double density, double low_edge_value, double high_edge_value)
{
 this->n_nodes = n_nodes;
 this->density = density;
 this->low_edge_value = low_edge_value;
 this->high_edge_value = high_edge_value;
 g = new graph(n_nodes, density, low_edge_value, high_edge_value);

 int non_zero_edges = (*g).num_non_zero_edges();
}

// destructor
short_path::~short_path()
{
 this->n_nodes = 0;
 this->density = 0.0;
 this->low_edge_value = 0.0;
 this->high_edge_value = 0.0;
 (*(this->g)).delete_all_edges();
 delete g;
}

// copy constructor
short_path::short_path(short_path & sp)
{
 this->density = sp.density;
 this->n_nodes = sp.n_nodes;
 this->low_edge_value = sp.low_edge_value; 
 this->high_edge_value = sp.high_edge_value;
 this->g = sp.g;
}


// check if graph is connected
void short_path::check_if_graph_connected()
{
 bool connected = (*g).is_connected();
 cout<<"is_connected(): "<<connected<<" \n";
}


// find path from u to v (i.e., all vertices)
vector<int> short_path::list_of_vertices(int u, int v)
{
 return (*g).find_path(u,v); 
}


// find path length from u to v
double short_path::path_length(int u, int v)
{
 vector<int> path = (*g).find_path(u,v);

 double path_l = 0.0; 

 if (path.size() > 0)
 {
   for (int i=0; i<path.size()-1; i++)
   {  
    path_l += (*g).get_edge_value(path[i], path[i+1]);
   }
 }

 return path_l; 
}

