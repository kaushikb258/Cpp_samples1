#include <iostream>
#include <vector>

#include "graph.h"
#include "queue.h"
#include "min_spanning.h"
#include "quick_sort.h"

using namespace std;


// constructor
min_spanning_tree::min_spanning_tree(int n_nodes, double density, double low_edge_value, double high_edge_value)
{
 this->n_nodes = n_nodes;
 this->density = density;
 this->low_edge_value = low_edge_value;
 this->high_edge_value = high_edge_value;
 g = new graph(n_nodes, density, low_edge_value, high_edge_value);

 

 int non_zero_edges = (*g).num_non_zero_edges();
}

// destructor
min_spanning_tree::~min_spanning_tree()
{
 this->n_nodes = 0;
 this->density = 0.0;
 this->low_edge_value = 0.0;
 this->high_edge_value = 0.0;
 (*(this->g)).delete_all_edges();
 delete g;
}

// copy constructor
min_spanning_tree::min_spanning_tree(min_spanning_tree & sp)
{
 this->density = sp.density;
 this->n_nodes = sp.n_nodes;
 this->low_edge_value = sp.low_edge_value; 
 this->high_edge_value = sp.high_edge_value;
 this->g = sp.g;
}


// kruskal
void min_spanning_tree::kruskal_mst()
{
 
 vector<double> edge_values;
 vector<int> node1, node2;
 
 int n_nodes = (*g).num_vertices();

 for (int i=0; i<n_nodes; i++)
 {
  for (int j=i+1; j<n_nodes; j++)
  {
   if (i != j && (*g).get_edge_value(i,j) > 0.0)
   {
    edge_values.push_back((*g).get_edge_value(i,j));
    node1.push_back(i);
    node2.push_back(j);
   }
  }
 }


 // call quicksort (pass by reference: so it gets mutated)
 quickSort(edge_values, node1, node2, 0, edge_values.size()-1); 


 cout<<"number of edges in open set: "<<edge_values.size()<<" \n";

 // create closet set of nodes
 vector<vector<int> > closed_set(n_nodes);
 for (int i=0; i<n_nodes; i++)
    closed_set[i].resize(n_nodes);

 for (int i=0; i<n_nodes; i++)
 {
  for (int j=0; j<n_nodes; j++)
  {
   if (j == 0) { closed_set[i][j] = i;}
   else {closed_set[i][j] = -1;}
  }
 }


 int idx = 0;
 bool result;

 vector<int> closed_set_node1, closed_set_node2;
 vector<double> closed_set_edge;

 int iclosed = 0;

 do
 {
  // check if edge lies in closed set
  // this occurs if for edge (i,j), i lies in closed_set_node1 and j in closed_set_node2, or vice versa  
 
  cout<<"---------\n";
  
  result = check_edge(closed_set, node1[idx], node2[idx]);

  if (result)
  {  
   iclosed++;
   closed_set_node1.push_back(node1[idx]);
   closed_set_node2.push_back(node2[idx]);
   closed_set_edge.push_back((*g).get_edge_value(node1[idx],node2[idx]));
  }

  idx++;
 } while(idx < edge_values.size() && closed_set.size() > 1);

   
 cout<<"---------\n";
 for (int i=0; i < closed_set[0].size(); i++)
   cout<<closed_set[0][i]<<" ";
 cout<<"\n";
 cout<<"---------\n"; 
 cout<<"---------\n";


 cout<<"idx: "<<idx<<"\n";
 cout<<"iclosed: "<<iclosed<<"\n";

 cout<<"number of entries in closed set: "<<closed_set_node1.size()<<"\n";

 // when we come here, closed set should have the minimum spanning tree.

 double total_mst = 0.0;


 cout<<"\n";
 cout<<"edges corresponding to minimum spanning tree: \n";
 for (int i=0; i<closed_set_node1.size(); i++)
 {
  cout<<"("<<closed_set_node1[i]<<" , "<<closed_set_node2[i]<<"); ";
  total_mst += closed_set_edge[i];
 }

 cout<<"number of edges in MST: "<<closed_set_node1.size()<<"\n";

 cout<<"\n";
 cout<<"total sum of edges of minimum spanning tree: "<<total_mst<<"\n";

}


bool min_spanning_tree::check_edge(vector<vector<int> > &closed_set, int node1, int node2)
{
 int r1, r2, j;
  

 for (int i=0; i<closed_set.size(); i++)
 {
  for (j=0; j<closed_set[i].size(); j++)
  {
   cout<<closed_set[i][j]<<" ";
  }
  cout<<"\n";
 }



 // find row where node1 exists
 for (int i=0; i<closed_set.size(); i++)
 {
  j = 0;
  while(closed_set[i][j] != -1 && j < closed_set[0].size())
  {
   if (closed_set[i][j] == node1) {r1 = i;}
   j++;
  }  
 }


 // find row where node2 exists
 for (int i=0; i<closed_set.size(); i++)
 {
  j = 0;
  while(closed_set[i][j] != -1 && j < closed_set[0].size())
  {
   if (closed_set[i][j] == node2) {r2 = i;}
   j++;
  }  
 } 


 if (r1 == r2)
 {
  // nodes 1 and 2 belong to same tree
  return false; 
 }


 // if we are here, it means nodes 1 and 2 belong to different trees  
 // merge rows r1 and r2 into one

 int c1, c2;
 
 j = 0;
 while(j < closed_set[0].size())
 {
  if (closed_set[r1][j] == -1)
  {
   c1 = j;
   break;
  }
  j++;
 }

 j = 0;
 while(j < closed_set[0].size())
 {
  if (closed_set[r2][j] == -1)
  {
   c2 = j;
   break;
  }
  j++;
 }


 // merge rows
 for (j=1; j<=c2; j++)
 {
  closed_set[r1][c1+j-1] = closed_set[r2][j-1];
 }

 closed_set.erase(closed_set.begin() + r2);

 return true;
}


