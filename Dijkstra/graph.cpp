#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm> 

#include "graph.h"
#include "queue.h"


using namespace std;

// overloaded constructors

graph::graph()
{
 this->n_nodes = 0;
 this->density = 0.0;
 this->low_edge_value = 0.0;
 this->high_edge_value = 0.0;
 edges = {};
}

graph::graph(unsigned int n, double density, double low_edge_value, double high_edge_value)
{ 
 this->n_nodes = n;
 this->density = density;
 this->low_edge_value = low_edge_value;
 this->high_edge_value = high_edge_value;
 edges = random_edges(); 

 /*
 for (int i=0; i<edges.size(); i++)
 {
  for (int j=0; j<edges.size(); j++)
  {
   cout<<i<<" "<<j<<" "<<edges[i][j]<<"\n";
  } 
  cout<<"\n";
 }
 */

}

// destructor
graph::~graph()
{ 
 delete_all_edges();
 this->density = 0.0;
 this->n_nodes = 0;
 this->low_edge_value = 0.0;
 this->high_edge_value = 0.0;
}

// copy constructor
graph::graph(graph &g)
{
 this->density = g.density;
 this->n_nodes = g.n_nodes;
 this->low_edge_value = g.low_edge_value; 
 this->high_edge_value = g.high_edge_value;

 for (int i=0; i<this->n_nodes; i++)
 {
  for (int j=0; j<this->n_nodes; j++)
  {
   this->edges[i][j] = g.edges[i][j];
  }
 }
}


// create random edges
vector<vector<double> > graph::random_edges()
{

 vector<vector<double> > e(this->n_nodes);
 double rand_num;

 for (int i=0; i<this->n_nodes; i++)
    e[i].resize(this->n_nodes); 


 const unsigned int seed = time(0);
 
 // Generating random numbers with C++11's random requires an engine and a distribution.
 // This is an engine based on the Mersenne Twister 19937 (64 bits):
 mt19937_64 rng(seed);
 

 uniform_real_distribution<double> distribution(0.0,1.0);
 


 for (int i=0; i<this->n_nodes; i++)
 {
  for (int j=i; j<this->n_nodes; j++)
  {

   if (i == j) { e[i][j] = 0.0; e[j][i] = 0.0;}

   rand_num = distribution(rng);

   if (i != j && rand_num <= this->density)
   {
    e[i][j] = this->low_edge_value + distribution(rng)*(this->high_edge_value - this->low_edge_value);  
    e[j][i] = e[i][j];  
   }

  }
 }

 return e;
}


// delete all edges
void graph::delete_all_edges()
{
 for (int i=0; i<this->n_nodes; i++)
 {
  edges[i].clear();
 }
 edges.clear();
}


// find number of non-zero valued edges
int graph::num_non_zero_edges()
{
 int non_zero = 0;

 for (int i=0; i<this->n_nodes; i++)
 {
  for (int j=i; j<this->n_nodes; j++)
  { 
   if (edges[i][j] > 0.0)
   {
    non_zero++;    
   }
  }
 }

 //cout<<"number of non-zero valued edges: "<<non_zero<<" \n";
 return non_zero;
}


// find number of vertices in graph
int graph::num_vertices()
{
 return edges.size();
}


// check if there is an edge between nodes x and y
bool graph::adjacent(int x, int y)
{
 if (x < this->n_nodes && y < this->n_nodes)
 {
  if (edges[x][y] > 0.0) {return true;}
 }
 return false;
}


// add edge between nodes x and y if it is not there
void graph::add_edge(int x, int y, double v)
{
 if (!this->adjacent(x,y)) 
 {
  edges[x][y] = edges[y][x] = v;
 } 
}


// delete an edge if it exists
void graph::delete_edge(int x, int y)
{
 if (this->adjacent(x,y))
 {
  edges[x][y] = edges[y][x] = 0.0;
 }
}


// get value of edge x, y
double graph::get_edge_value(int x, int y)
{
 return edges[x][y];
}

// return vector containing indices of my neighbors
vector<int> graph::my_neighbors(int me)
{
 vector<int> neighbors;
 for (int i=0; i<this->n_nodes; i++)
 {
  if (edges[i][me] > 0.0001 && i != me) { neighbors.push_back(i); }
 }
 return neighbors;
}


// check if graph is connected (i.e., all nodes connected)
bool graph::is_connected()
{

 queue closed_set, open_set; 

 // start from node 0 and proceed as suggested in Prof. Pohl's video

 int iter = 0;
 int top_node;
 double top_node_value;
 int dummy;

 // the prev node in the is_connected() algorithm is dummy; so set them to node itself!


 while (closed_set.size() < this->n_nodes)
 {
 
  if (iter == 0)
  {
   // insert first node, i.e., node 0 into closed set with value = 0
   closed_set.Insert(0,0.0,0);

   // insert neighbors of node 0 into open set
   vector<int> neighbors = my_neighbors(0);
   for (int i=0; i<neighbors.size(); i++)
   {
    open_set.Insert(neighbors[i],edges[0][neighbors[i]],neighbors[i]); 
   }
   neighbors.clear();
  }

  else
  {  
   if (open_set.size() == 0) {break;}

   // find the node in open set with least value and add it to closed set
   open_set.minPriority(top_node, top_node_value, dummy);

   // for is_connected, we only care about nodes, not their edge values; 
   // so the top_node_value is dummy here;
   // for shortest path we will need it though 
   closed_set.Insert(top_node, top_node_value, top_node);

   // add neighbors of top_node to open set that are not already part of the closed set or open set
   vector<int> neighbors = my_neighbors(top_node);
   for (int i=0; i<neighbors.size(); i++)
   {
    if (!closed_set.contains(neighbors[i]) && !open_set.contains(neighbors[i]))
    {
     open_set.Insert(neighbors[i],edges[top_node][neighbors[i]], neighbors[i]); 
    }
   }
   neighbors.clear();

  }
  
  iter++;

 }
 
 if (closed_set.size() == this->n_nodes) {return true;}
 if (closed_set.size() < this->n_nodes && open_set.size() == 0) {return false;}

 cout<<"shoud not be here \n";
}



// find path between nodes u and v
vector<int> graph::find_path(int u, int v)
{
 queue closed_set, open_set; 

 // start from node u and proceed as suggested in Prof. Pohl's video

 int iter = 0;
 int top_node;
 double top_node_value;
 int top_node_prev;
 vector<int> path;
 bool path_exists = true;
 int current_node;

 while (closed_set.size() < this->n_nodes)
 {
 
  if (iter == 0)
  {
   // insert first node, i.e., node u into closed set with value = 0, prev = u
   closed_set.Insert(u,0.0,u);

   // insert neighbors of node u into open set
   vector<int> neighbors = my_neighbors(u);
   for (int i=0; i<neighbors.size(); i++)
   {
    open_set.Insert(neighbors[i],edges[u][neighbors[i]],u); 
   }
   neighbors.clear();
  }

  else
  {  
   if (open_set.size() == 0)
   {
    // no path exists from u to v 
    // so set path vector to empty
    path.erase(path.begin(), path.end()); 
    path_exists = false;
    return path;
    break; //  we don't need this break! 
   }

   // find the node in open set with least value and add it to closed set
   open_set.minPriority(top_node, top_node_value, top_node_prev);

   if (open_set.contains(top_node)) {cout<<"bug "<<top_node<<"\n";}

   closed_set.Insert(top_node, top_node_value, top_node_prev);


   if (top_node == v)
   {
    // found path from u to v
    path_exists = true;
    break;
   }

   // add neighbors of top_node to open set that are not already part of the closed set or may exist in open set with larger value
   vector<int> neighbors = my_neighbors(top_node);
   for (int i=0; i<neighbors.size(); i++)
   {
    if (!closed_set.contains(neighbors[i]))    
    {

     if (!open_set.contains(neighbors[i]))  
         open_set.Insert(neighbors[i],top_node_value+edges[top_node][neighbors[i]],top_node); // top_node becomes prev node

     else
     { 
      if (top_node_value+edges[top_node][neighbors[i]] < open_set.get_value(neighbors[i]))  
          open_set.chgPrioirity(neighbors[i], top_node_value+edges[top_node][neighbors[i]], top_node);
     }

    }
   }
   neighbors.clear();

  }
  
  iter++;

 }
 

 if (path_exists)
 {
  // sweep through the closed set to find reverse path
  current_node = v;
  while(current_node != u)
  {
   path.push_back(current_node);
   current_node = closed_set.get_prev(current_node);  
  }
 }
 path.push_back(current_node);

 // reverse path to make it u -> v
 reverse(path.begin(), path.end()); 

 return path;
}

