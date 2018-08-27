#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "graph.h"
#include "shortest_path.h"

using namespace std;


int main()
{

 unsigned int n_nodes = 50; // total number of nodes
 double density = 0.2; // density of edges
 double low_edge_value = 1.0; // low value
 double high_edge_value = 10.0; // high value

 cout<<"number of nodes: "<<n_nodes<<"\n";
 cout<<"density: "<<density<<"\n";
 cout<<"low and high edge values: "<<low_edge_value<<" "<<high_edge_value<<"\n";


 // short_path object
 short_path shp(n_nodes, density, low_edge_value, high_edge_value);
 
 // check if is_connected
 shp.check_if_graph_connected();
 

 const unsigned int seed = time(0);
 
 // Generating random numbers with C++11's random requires an engine and a distribution.
 // This is an engine based on the Mersenne Twister 19937 (64 bits):
 mt19937_64 rng(seed);
 
 uniform_int_distribution<int> dis(0, n_nodes-1);

 int n1, n2;
 do
 {
  n1 = dis(rng);
  n2 = dis(rng);
 } while (n1 == n2);


 // find path between two random nodes n1 and n2
 double path = shp.path_length(n1, n2);
 cout<<"path length between nodes "<<n1<<" and "<<n2<<": "<<path<<" \n";


 // find list of vertices in the shortest path between n1 and n2
 vector<int> vert = shp.list_of_vertices(n1, n2);

 cout<<"list of vertices: \n";
 for (int i=0; i<vert.size(); i++)
   cout<<vert[i]<<" ";
 cout<<"\n";




 // compute average path length for node pairs (0,1), (0,2), ..., (0,49)
 double avg_path_length = 0.0;
 double num_attempts = 0.0;
 for (int i=1; i<n_nodes; i++)
 {
  short_path shp2(n_nodes, density, low_edge_value, high_edge_value);
  path = shp.path_length(0, i);
  if (path > 0.0) {avg_path_length += path; num_attempts += 1.0;}
 }
 avg_path_length /= num_attempts;
 cout<<"number of monte carlo runs: "<<num_attempts<<"\n";
 cout<<"average path length: "<<avg_path_length<<"\n";
 

 cout<<"done! \n";
 
 return 0;
}
