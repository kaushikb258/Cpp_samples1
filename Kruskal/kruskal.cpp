#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "graph.h"
#include "min_spanning.h"

using namespace std;


int main()
{

 unsigned int n_nodes = 50; // total number of nodes
 double density = 1.0; // density of edges: note that for Kruskal, we like all edges to exist; so set density=1 
 double low_edge_value = 1.0; // low value
 double high_edge_value = 10.0; // high value

 cout<<"number of nodes: "<<n_nodes<<"\n";
 cout<<"density: "<<density<<"\n";
 cout<<"low and high edge values: "<<low_edge_value<<" "<<high_edge_value<<"\n";


 // min_spanning_tree object
 min_spanning_tree mst(n_nodes, density, low_edge_value, high_edge_value);
 

 // use Kruskal's algo to compute MST 
 mst.kruskal_mst();

 
 cout<<"done! \n";
 
 return 0;
}
