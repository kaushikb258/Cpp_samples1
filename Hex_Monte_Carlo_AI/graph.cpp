#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm> 
#include <stdlib.h>

#include "graph.h"
#include "queue.h"


using namespace std;

// constructor
graph::graph()
{
 this->n_nodes = 0;
 this->nodes = {};
 this->edges = {};
}

// constructor
graph::graph(int n)
{
 this->n_nodes = n;
 nodes.resize(n);
 
 edges.resize(n);
 for(auto it = edges.begin(); it != edges.end(); ++it)
 {
  it->resize(n);
 }
}

// destructor
graph::~graph()
{ 
 delete_all_edges();
 this->n_nodes = 0;
 this->nodes = {};
}

// copy constructor
graph::graph(graph &g)
{
 this->n_nodes = g.n_nodes;

 this->nodes.resize(this->n_nodes);
 this->edges.resize(this->n_nodes);
 for(auto it = edges.begin(); it != edges.end(); ++it)
 {
  it->resize(this->n_nodes);
 }


 for (int i=0; i<this->n_nodes; i++)
 {
  this->nodes[i] = g.nodes[i];
  for (int j=0; j<this->n_nodes; j++)
  {
   this->edges[i][j] = g.edges[i][j];
  }
 }
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



// check if there is an edge between nodes x and y
// x and y are node numbers, not indices
bool graph::adjacent(int x, int y)
{
 // first check if x and y belong in nodes

 bool xexists = false;
 for (int i=0; i<nodes.size(); i++)
 {
  if (nodes[i] == x)
     xexists = true;
 } 

 bool yexists = false;
 for (int i=0; i<nodes.size(); i++)
 {
  if (nodes[i] == y)
     yexists = true;
 } 

 if (xexists == false || yexists == false) {return false;}

 if (edges[get_index(x)][get_index(y)] > 0) {return true;}
 else {return false;}
}


// add a new node to graph
void graph::add_node(int n)
{
 this->n_nodes += 1;

 nodes.push_back(n);

 vector<int> a;
 for (int i=0; i<this->n_nodes-1; i++) {a.push_back(0);} 
 edges.push_back(a);

 for (int i=0; i<this->n_nodes; i++)
 {
  edges[i].push_back(0);
 }
}

// add edge between nodes x and y if it is not there
// x and y are node numbers, not indices
void graph::add_edge(int x, int y)
{
 if (!this->adjacent(x,y)) 
 {
  edges[get_index(x)][get_index(y)] = edges[get_index(y)][get_index(x)] = 1;
 } 
}


// delete edge between nodes x and y 
// x and y are node numbers, not indices
void graph::del_edge(int x, int y)
{
  edges[get_index(x)][get_index(y)] = edges[get_index(y)][get_index(x)] = 0; 
}



vector<int> graph::get_nodes()
{
 return this->nodes;
}


// return vector containing global indices of my neighbors
// me is node number, not index
// outputs neighbors node numbers, not indices
vector<int> graph::my_neighbors(int me)
{

 // first check if me exists in nodes
 bool me_exists = false;
 for (int i=0; i<nodes.size(); i++)
 {
  if (nodes[i] == me)
     me_exists = true;
 } 
 
 if (me_exists == false)
 {
  cout<<"me does not exist in nodes "<<me<<"\n";
  system("pause"); 
 }

 vector<int> neighbors;
 for (int i=0; i<this->n_nodes; i++)
 {
  if (edges[i][get_index(me)] > 0 && nodes[i] != me) { neighbors.push_back(nodes[i]); }
 }
 return neighbors;
}


// get index of node
int graph::get_index(int node_)
{
 int ind = -1;
 for (int i=0; i<nodes.size(); i++)
 {
  if (nodes[i] == node_) {ind = i;}
 } 

 if (ind == -1)
 {
  cout<<"index error "<<ind<<"\n";
  system("pause");
 }

 return ind;
}



// check if graph is connected (i.e., all nodes connected)
bool graph::is_connected()
{

 queue closed_set, open_set; 

 // start from node 0 and proceed as suggested in Prof. Pohl's video

 int iter = 0;
 int top_node;
 int top_node_value;
 int dummy;

 // the prev node in the is_connected() algorithm is dummy; so set them to node itself!


 while (closed_set.size() < this->n_nodes)
 {
 
  if (iter == 0)
  {
   // insert first node, i.e., nodes[0] into closed set with value = 0
   closed_set.Insert(nodes[0],0,nodes[0]);

   // insert neighbors of node 0 into open set
   vector<int> neighbors = my_neighbors(nodes[0]);
   for (int i=0; i<neighbors.size(); i++)
   {
    open_set.Insert(neighbors[i],edges[get_index(nodes[0])][get_index(neighbors[i])],neighbors[i]); 
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
     open_set.Insert(neighbors[i],edges[get_index(top_node)][get_index(neighbors[i])], neighbors[i]); 
    }
   }
   neighbors.clear();

  }
  
  iter++;

 }
 
 if (closed_set.size() == this->n_nodes) {return true;}
 if (closed_set.size() < this->n_nodes && open_set.size() == 0) {return false;}

 cout<<"shoud not be here \n";
 system("pause");
}



// find path between nodes u and v
vector<int> graph::find_path(int u, int v)
{
 queue closed_set, open_set; 

 // start from node u and proceed as suggested in Prof. Pohl's video

 int iter = 0;
 int top_node;
 int top_node_value;
 int top_node_prev;
 vector<int> path;
 bool path_exists = true;
 int current_node;

 
 while (closed_set.size() < this->n_nodes)
 {
 
  if (iter == 0)
  {
   // insert first node, i.e., node u into closed set with value = 0, prev = u
   closed_set.Insert(u,0,u);
  
   
   // insert neighbors of node u into open set
   vector<int> neighbors = my_neighbors(u);
   
   
   for (int i=0; i<neighbors.size(); i++)
   {
    open_set.Insert(neighbors[i],edges[get_index(u)][get_index(neighbors[i])],u); 
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

   if (open_set.contains(top_node)) {cout<<"bug "<<top_node<<"\n"; system("pause");}

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
         open_set.Insert(neighbors[i],top_node_value+edges[get_index(top_node)][get_index(neighbors[i])],top_node); // top_node becomes prev node

     else
     { 
      if (top_node_value+edges[get_index(top_node)][get_index(neighbors[i])] < open_set.get_value(neighbors[i]))  
          open_set.chgPrioirity(neighbors[i], top_node_value+edges[get_index(top_node)][get_index(neighbors[i])], top_node);
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

