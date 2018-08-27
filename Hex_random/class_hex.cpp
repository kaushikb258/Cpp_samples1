#include <iostream>
#include <random>
#include <stdlib.h>

#include "class_hex.h"
#include "queue.h"
#include "graph.h"



using namespace std;



hex_board::hex_board(int n)
{
 nside = n;

 vector<vector<int> > matrix(nside);
 for (int i = 0 ; i < nside; i++)
   matrix[i].resize(nside);

 node_occupancy = matrix;

 //player1 = graph(0);
 //player2 = graph(0);
 // for hex_global, node number = index
 // not true for player1 and player2 
 //hex_global = graph(0);

 for (int i=0; i<n*n; i++)
   hex_global.add_node(i);

}

hex_board::~hex_board()
{
 empty_nodes();
}

void hex_board::empty_nodes()
{
 for (int i=0; i<nside; i++)
 {
  node_occupancy[i].clear();
 }
 node_occupancy.clear();

 player1.delete_all_edges();
 player2.delete_all_edges();
 hex_global.delete_all_edges();   

 nside = 0;
}

int hex_board::check_node(int r, int c) 
{
 return node_occupancy[r][c];
}

void hex_board::occupy_node(int r, int c, int player)
{
 node_occupancy[r][c] = player;  
 
 if (player == 1) 
   player1.add_node(c + r*nside);
 else if (player == 2)
   player2.add_node(c + r*nside);

}


void hex_board::print()
{
 cout<<"hex board: \n";
 cout<<"-------------------------\n";
 for (int i=0; i<nside; i++)
 {
  for (int j=0; j<nside; j++)
  {
   cout<<node_occupancy[i][j]<<" ";
  }
  cout<<"\n";
  if (i < nside-1) { for (int k=0; k<i+1; k++) cout<<" "; }
 }
 cout<<"-------------------------\n";
}


bool hex_board::hex_board_full()
{
 for (int i=0; i<nside; i++)
 {
  for (int j=0; j<nside; j++)
  {
   if (node_occupancy[i][j] == 0) return false;
  }
 }
 return true;
}




void hex_board::play_hex(bool human_entry)
{

 std::random_device rd;     // only used once to initialise (seed) engine
 std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
 std::uniform_int_distribution<int> uni(0,nside); // guaranteed unbiased


 int player_turn = 1;
 int r, c;

 this->print(); 

 this->set_hex_global();

 while(!this->hex_board_full() && !this->check_end_of_game())
 {
  cout<<"player "<<player_turn<<"'s turn \n";

  if (human_entry)
  {
   cout<<"enter row and column (space separated): \n";
   cin>>r>>c;
  }
  else
  {
   r = uni(rng);
   c = uni(rng);
   cout<<"r = "<<r<<"; c = "<<c<<"\n";
  }


  if (r < 0 || c < 0 || r > nside-1 || c > nside-1) {cout<<"invalid entry, please try again \n";} 
  else if (this->check_node(r,c) == 1 || this->check_node(r,c) == 2) {cout<<"node occupied, please try again \n";}   
  else 
  { 
   // valid entry
   cout<<"valid entry \n";
   this->occupy_node(r,c,player_turn);
   this->print(); 
   player_turn = (player_turn % 2)+1;
  }
 }

 cout<<"game ended \n";
}


void hex_board::set_hex_global()
{
 int nn = nside*nside;
 vector<vector<int> > ee(nn);
 for (int i = 0; i < nn; i++)
   ee[i].resize(nn);
 
 // set all edges to zero
 for (int i=0; i<nn; i++)
 {
  for (int j=0; j<nn; j++)
  {
   ee[i][j] = 0;
  }
 }

 // create hex board edges
 for (int i=0; i<nn-1; i++)
 {
  if ((i+1)%nside != 0) 
  {
   ee[i][i+1] = ee[i+1][i] = 1;
  }
  if (i <= nn-nside-1)
  {
   ee[i][i+nside] = ee[i+nside][i] = 1;
  }
 }


 // diagonals
 for (int i=1; i<=nn-nside-1; i++) 
 {
  if (i%nside != 0)
    ee[i][i+nside-1] = ee[i+nside-1][i] = 1;
 }

 // note: for hex_global the (i,j) can be indices as index = node number
 // for player1 and player2, this is not true
 for (int i=0; i<nn; i++)
 {
  for (int j=0; j<nn; j++) 
  { 
   if (ee[i][j] == 1) { hex_global.add_edge(i,j); }
  }
 }

}




bool hex_board::check_end_of_game()
{

 bool empty_node = false;

 for (int i=0; i<node_occupancy.size(); i++)
 {
  for (int j=0; j<node_occupancy[0].size(); j++)
  {
   if (node_occupancy[i][j] == 0)
   {
    empty_node = true;
   }
  }
 }

 // game ended due to no more empty nodes available
 if (empty_node == false)
    return true;

 // set edges for players 1 and 2
 this->set_player_edges();

 // check to see if path exists from N-S for player1 
 if (this->check_north_south_path_for_player1())
 {
  cout<<"******************************************\n";
  cout<<"player1 (North-South) wins \n";  
  cout<<"******************************************\n";
  return true;
 }
 

 // check to see if path exists from E-W for player2 
 if (this->check_east_west_path_for_player2())
 {
  cout<<"******************************************\n";
  cout<<"player2 (East-West) wins \n";
  cout<<"******************************************\n";
  return true;
 }


 return false;
}




//---------------------------------------------------------------------------------------------

void hex_board::set_player_edges()
{
 
 vector<int> p1_nodes = player1.get_nodes();

 for (int i=0; i<p1_nodes.size(); i++)
 {
  for (int j=0; j<p1_nodes.size(); j++)
  {
   if (i != j)
   {
    if (hex_global.adjacent(p1_nodes[i],p1_nodes[j]))
    {
     player1.add_edge(p1_nodes[i],p1_nodes[j]);
    } 
   }
  }
 }


 vector<int> p2_nodes = player2.get_nodes();

 for (int i=0; i<p2_nodes.size(); i++)
 {
  for (int j=0; j<p2_nodes.size(); j++)
  {
   if (i != j)
   {
    if (hex_global.adjacent(p2_nodes[i],p2_nodes[j]))
    {
     player2.add_edge(p2_nodes[i],p2_nodes[j]);
    } 
   }
  }
 }


 cout<<"player1 nodes: ";
 for (int i=0; i<p1_nodes.size(); i++) {cout<<p1_nodes[i]<<" ";}
 cout<<"\n";
 cout<<"player2 nodes: ";
 for (int i=0; i<p2_nodes.size(); i++) {cout<<p2_nodes[i]<<" ";}
 cout<<"\n";

}




bool hex_board::check_north_south_path_for_player1()
{
 vector<int> north, south;
 vector<int> p1_nodes = player1.get_nodes();

 for (int i=0; i<p1_nodes.size(); i++)
 { 
  if (p1_nodes[i] < nside)
     north.push_back(p1_nodes[i]);
  else if (p1_nodes[i] >= nside*nside - nside)
     south.push_back(p1_nodes[i]);
 }
 
 
 // for every north-south pair of nodes, check if a path exists
 vector<int> path;
 for (int i = 0; i<north.size(); i++)
 {
  for (int j=0; j<south.size(); j++)
  {
   path = player1.find_path(north[i],south[j]);
   if (path.size()>0)
   {
    cout<<"found north-south path between nodes "<<north[i]<<" "<<south[j]<<" \n";
    cout<<"path: ";
    for (auto k: path)
      cout<<k<<" ";
    cout<<"\n";
    return true;
   }
  }
 } 

 return false; 
}



bool hex_board::check_east_west_path_for_player2()
{
 vector<int> east, west;
 vector<int> p2_nodes = player2.get_nodes();

 for (int i=0; i<p2_nodes.size(); i++)
 { 
  if (p2_nodes[i] % nside == 0)
     west.push_back(p2_nodes[i]);
  else if ((p2_nodes[i]+1) % nside == 0)
     east.push_back(p2_nodes[i]);
 }
 
 
 // for every east-west pair of nodes, check if a path exists
 vector<int> path;
 for (int i = 0; i<east.size(); i++)
 {
  for (int j=0; j<west.size(); j++)
  {
   path = player2.find_path(east[i],west[j]);
   if (path.size()>0)
   {
    cout<<"found east-west path between nodes "<<east[i]<<" "<<west[j]<<" \n";
    cout<<"path: ";
    for (auto k: path)
      cout<<k<<" ";
    cout<<"\n";
    return true;
   }
  }
 } 


 return false; 
}



