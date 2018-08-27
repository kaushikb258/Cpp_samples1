#include <iostream>
#include <random>
#include <algorithm>
#include <stdlib.h>
#include <chrono>
#include <ctime>

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


//------------------------------------------------------------------------------


void hex_board::play_hex(bool ai_hex)
{
 int player_turn;
 int r, c, ii;
 int human, plyr;
 vector<int> remaining_nodes;
 

 // toss a coin to determine who starts first
 std::default_random_engine generator;
 std::uniform_int_distribution<int> distribution(1,2);

 player_turn = distribution(generator);
 cout<<"game started by player "<<player_turn<<"\n";


 

 for (int i=0; i<nside*nside; i++)
    remaining_nodes.push_back(i);



 while(true)
 {
  cout<<"enter human player (1/2): \n";
  cin>>human;
  if (human == 1 || human == 2)
        break;
  cout<<"wrong entry \n";
 } 
 

 this->print(); 

 this->set_hex_global();


 while(!this->hex_board_full() && !this->check_end_of_game(plyr, node_occupancy, player1, player2))
 {
  cout<<"player "<<player_turn<<"'s turn \n";

  if (ai_hex == false)
  {
   // human versus idiot player

    ii = -1;

    if (player_turn == human)
    {
     // for the time being use machine entry for human as well; human is lazy to type node number
     //human_entry(remaining_nodes, ii);
     machine_entry(remaining_nodes, ii);  
    }
    else
    {
     machine_entry(remaining_nodes, ii);  
    }

    if (ii == -1) 
    {
     cout<<"error in ii "<<ii<<"\n";
     system("pause");
     abort();
    }
  }
  else
  {
   // human versus intelligent AI player 
   
    // first node selection random for each player; then only AI starts
    if (remaining_nodes.size() < nside*nside-2)
    {
      if (player_turn == human)
      {
       // for the time being use AI machine entry for human as well; human is lazy to type node number
       //human_entry(remaining_nodes, ii);
       ii = ai_choose_node(remaining_nodes, human, player_turn);

       // for the time being use one player as idiot, to speed up the simulation 
       //machine_entry(remaining_nodes, ii);
      }
      else
      {
       std::time_t tstart = std::time(nullptr); 
       // AI agent to determine next node on hex board to choose
       ii = ai_choose_node(remaining_nodes, human, player_turn);  
       std::time_t tend = std::time(nullptr);
       cout<<"time (sec): "<<tend-tstart<<"\n";
      }
    }
    else
    { machine_entry(remaining_nodes, ii); } 
  }

  r = remaining_nodes[ii]/nside;
  c = remaining_nodes[ii]%nside;
  cout<<"r = "<<r<<"; c = "<<c<<"\n";
 

  if (ii == -1) 
  {
   cout<<"error in ii (palo) "<<ii<<"\n";
   system("pause");
   abort();  
  }


  remaining_nodes.erase(remaining_nodes.begin() + ii);
 

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

 
 if (plyr == 1) cout<<"player 1 won \n";
 else if (plyr == 2) cout<<"player 2 won \n";
 else {cout<<"plyr "<<plyr<<"\n"; abort();}
   
 
 cout<<"game ended \n";
}


//------------------------------------------------------------------

// AI choosing of node
int hex_board::ai_choose_node(vector<int> remaining_nodes, int human, int player_turn)
{

 int r, c, ibest, ibest_opponent;
 vector<int> rem_nodes; 
 double frac_player1, frac_player2; 
 double best_for_me = -1.0; 
 double best_for_opponent = -1.0;

 // temporary variables
 vector<vector<int> > node_occupancy_; 
 graph player1_, player2_; 
 int player_turn_;

 if (remaining_nodes.size() <= 0)
 {
  cout<<"ai_choose_node not possible "<<remaining_nodes.size()<<"\n";
  system("pause");
  abort();
 }

 //------------best for me------------------------------- 
 ibest = -1;

 for (int i=0; i<remaining_nodes.size(); i++)
 {
  node_occupancy_ = node_occupancy;
  player1_ = player1;
  player2_ = player2; 
  player_turn_ = player_turn;
 
 
  r = remaining_nodes[i]/nside;
  c = remaining_nodes[i]%nside;
  rem_nodes = remaining_nodes; 
  rem_nodes.erase(rem_nodes.begin() + i);

 
  node_occupancy_[r][c] = player_turn_;  
  if (player_turn_ == 1) 
    player1_.add_node(c + r*nside);
  else if (player_turn_ == 2)
    player2_.add_node(c + r*nside);
  player_turn_ = (player_turn_ % 2)+1;


  frac_player1 = two_idiots(rem_nodes, human, player_turn_, player1_, player2_, node_occupancy_);
  frac_player2 = 1.0 - frac_player1;

  if (frac_player1 < 0.0 || frac_player1 > 1.0 || frac_player2 < 0.0 || frac_player2 > 1.0)
  {
   cout<<"fractions out of bounds "<<frac_player1<<" "<<frac_player2<<"\n";
   system("pause");
   abort();
  }



  if (player_turn == 1 && frac_player1 > best_for_me)
  {
   best_for_me = frac_player1;
   ibest = i;
  }
  else if (player_turn == 2 && frac_player2 > best_for_me)
  {
   best_for_me = frac_player2;
   ibest = i;
  }

 } 

 if (ibest == -1)
 {
  cout<<"ibest error in ai: "<<ibest<<"\n";
  abort();
 }
 //------------best for me------------------------------- 


 //------------best for opponent------------------------------- 
 ibest_opponent = -1;


 for (int i=0; i<remaining_nodes.size(); i++)
 {
  node_occupancy_ = node_occupancy;
  player1_ = player1;
  player2_ = player2; 
  player_turn_ = player_turn;
  
  // play the game in the opponent's shoes and see what is best for him
  player_turn_ = (player_turn_ % 2)+1; 

 
  r = remaining_nodes[i]/nside;
  c = remaining_nodes[i]%nside;
  rem_nodes = remaining_nodes; 
  rem_nodes.erase(rem_nodes.begin() + i);

 
  node_occupancy_[r][c] = player_turn_;  
  if (player_turn_ == 1) 
    player1_.add_node(c + r*nside);
  else if (player_turn_ == 2)
    player2_.add_node(c + r*nside);
  player_turn_ = (player_turn_ % 2)+1;


  frac_player1 = two_idiots(rem_nodes, human, player_turn_, player1_, player2_, node_occupancy_);
  frac_player2 = 1.0 - frac_player1;

  if (frac_player1 < 0.0 || frac_player1 > 1.0 || frac_player2 < 0.0 || frac_player2 > 1.0)
  {
   cout<<"fractions out of bounds "<<frac_player1<<" "<<frac_player2<<"\n";
   system("pause");
   abort();
  }

  
  if (player_turn == 1 && frac_player2 > best_for_opponent)
  {
   best_for_opponent = frac_player2;
   ibest_opponent = i;
  }
  else if (player_turn == 2 && frac_player1 > best_for_opponent)
  {
   best_for_opponent = frac_player1;
   ibest_opponent = i;
  }

 } 

 if (ibest_opponent == -1)
 {
  cout<<"ibest_opponent error in ai: "<<ibest_opponent<<"\n";
  abort();
 }
 //------------best for opponent------------------------------- 


 // we must decide if we are to choose what is best for me and exploit (greedy strategy)
 // or what is best for opponent and stop him from winning (blocking strategy)

 cout<<"best for me: "<<best_for_me<<"\n";
 cout<<"best for opponent: "<<best_for_opponent<<"\n";

 cout<<"ibest/ibest_opponent: "<<ibest<<" "<<ibest_opponent<<"\n";

 if (best_for_me >= 0.8*best_for_opponent)
    return ibest;
 else
    return ibest_opponent;


}


//------------------------------------------------------------------
// monte carlo play
// let the two idiots play till end of the game; find who won and keep statistics

double hex_board::two_idiots(vector<int> rem_nodes, int human, int player_turn_, 
             graph player1_, graph player2_, vector<vector<int> > node_occupancy_)
{
  
 int monte_carlo = 100;
 int r, c, plyr;

 // temporary variables
 vector<int> rn;
 vector<vector<int> > node_occupancy__; 
 graph player1__, player2__;
 int player_turn__;

 std::random_device rd;
 std::mt19937 g(rd());
 
 int player1_wins = 0;
 int player2_wins = 0;


 for (int mc = 0; mc < monte_carlo; mc++)
 { 
 
  // play till end of game, as suggested by Prof. Ira Pohl

  // randomly shuffle remaining_nodes 
  rn = rem_nodes;
  std::shuffle(rn.begin(), rn.end(), g);     


  node_occupancy__ = node_occupancy_;
  player1__ = player1_;
  player2__ = player2_;
  player_turn__ = player_turn_;



  for (int i=0; i<rn.size(); i++)
  {
   // randomly choose an unoccupied node, say rn[i]
   r = rn[i]/nside;
   c = rn[i]%nside;
   
   if (r < 0 || c < 0 || r > nside-1 || c > nside-1) {cout<<"invalid entry (2idiots) \n"; abort(); system("pause");} 
   else 
   { 
    node_occupancy__[r][c] = player_turn__;  
    if (player_turn__ == 1) 
      player1__.add_node(c + r*nside);
    else if (player_turn__ == 2)
      player2__.add_node(c + r*nside);
    player_turn__ = (player_turn__ % 2)+1;
   }
  }


  // check Dijkstra-style end of game, only once per monte carlo iteration
  bool end_of_game = this->check_end_of_game(plyr, node_occupancy__, player1__, player2__); 

  if (end_of_game == false)
  {
   cout<<"something wrong - game must have ended by now "<<end_of_game<<"\n";
   system("pause");
   abort();
  }


  if (plyr == 1) player1_wins++;
  else if (plyr == 2) player2_wins++;
  else {cout<<"bug in plyr (2idiots) "<<plyr<<"\n"; abort(); system("pause");} 

 }

  return (static_cast<double>(player1_wins)/static_cast<double>(monte_carlo)); 

}

//------------------------------------------------------------------
// set the global edges for the hex board

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


//--------------------------------------------------------------------
// human choosing an entry
bool hex_board::human_entry(vector<int> remaining_nodes, int &ii)
{
 int n, r, c, i;
 bool found = false;

 ii = -1;

 while(true)
 {
  cout<<"enter row and column (space separated): \n";
  cin>>r>>c;
  n = c + r*nside;
  if (r >= 0 && c >= 0 && r < nside && c < nside)
  {
    for (i=0; i<remaining_nodes.size(); i++)
    {
     if (remaining_nodes[i] == n) {found = true; ii = i;}
    }
    if (found)
      break;
    cout<<"node occupied; please enter another value \n";
  }
  else
  {cout<<"invalid entry \n";}
 }

 return found; 
}


// machine choosing an entry
bool hex_board::machine_entry(vector<int> remaining_nodes, int &ii)
{
 int n;
 bool found = false;

 std::random_device rd;     // only used once to initialise (seed) engine
 std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
 std::uniform_int_distribution<int> uni(0,remaining_nodes.size()-1); // guaranteed unbiased

 if (remaining_nodes.size() <= 0)
 {
  cout<<"board full \n"; 
  system("pause");
  abort();
 }
 else {ii = uni(rng); found = true;} 

 return found; 
}



//--------------------------------------------------------------------

bool hex_board::check_end_of_game(int &plyr, vector<vector<int> > node_occupancy__, graph &player1__, graph &player2__)
{

 bool screen = false;
 bool empty_node = false;
 

 plyr = -1;

 for (int i=0; i<node_occupancy__.size(); i++)
 {
  for (int j=0; j<node_occupancy__[0].size(); j++)
  {
   if (node_occupancy__[i][j] == 0)
   {
    empty_node = true;
   }
  }
 }


 // game ended due to no more empty nodes available
 //if (empty_node == false)
 //   return true;

 // set edges for players 1 and 2
 this->set_player_edges(player1__, player2__, screen);



 // check to see if path exists from N-S for player1 
 if (player1__.get_nodes().size() >= 2 && this->check_north_south_path_for_player1(player1__, screen))
 {
  plyr = 1;
  return true;
 }
 

 // check to see if path exists from E-W for player2 
 if (player2__.get_nodes().size() >= 2 && this->check_east_west_path_for_player2(player2__, screen))
 {
  plyr = 2;
  return true;
 }

 return false;
}




//---------------------------------------------------------------------------------------------

void hex_board::set_player_edges(graph &player1__, graph &player2__, bool screen)
{
 
 vector<int> p1_nodes = player1__.get_nodes();

 // first set edge to zero
 for (int i=0; i<p1_nodes.size(); i++)
 {
  for (int j=0; j<p1_nodes.size(); j++)
  {
   player1__.del_edge(p1_nodes[i],p1_nodes[j]);
  }
 }


 for (int i=0; i<p1_nodes.size(); i++)
 {
  for (int j=0; j<p1_nodes.size(); j++)
  {
   if (i != j)
   {
    if (hex_global.adjacent(p1_nodes[i],p1_nodes[j]))
    {
     player1__.add_edge(p1_nodes[i],p1_nodes[j]);
    } 
   }
  }
 }





 vector<int> p2_nodes = player2__.get_nodes();

 // first set edge to zero
 for (int i=0; i<p2_nodes.size(); i++)
 {
  for (int j=0; j<p2_nodes.size(); j++)
  {
   player2__.del_edge(p2_nodes[i],p2_nodes[j]);
  }
 }
 
 for (int i=0; i<p2_nodes.size(); i++)
 {
  for (int j=0; j<p2_nodes.size(); j++)
  {
   if (i != j)
   {
    if (hex_global.adjacent(p2_nodes[i],p2_nodes[j]))
    {
     player2__.add_edge(p2_nodes[i],p2_nodes[j]);
    } 
   }
  }
 }

 if (screen)
 {
  cout<<"player1 nodes: ";
  for (int i=0; i<p1_nodes.size(); i++) {cout<<p1_nodes[i]<<" ";}
  cout<<"\n";
  cout<<"player2 nodes: ";
  for (int i=0; i<p2_nodes.size(); i++) {cout<<p2_nodes[i]<<" ";}
  cout<<"\n";
 }


}

//---------------------------------------------------------------------------------------------


bool hex_board::check_north_south_path_for_player1(graph player1__, bool screen)
{

 vector<int> north, south;
 vector<int> p1_nodes = player1__.get_nodes();

 for (int i=0; i<p1_nodes.size(); i++)
 { 
  if (p1_nodes[i] < nside)
     north.push_back(p1_nodes[i]);
  else if (p1_nodes[i] >= nside*nside - nside)
     south.push_back(p1_nodes[i]);
 }
 
 if (north.size() <= 0 || south.size() <= 0) {return false;}

 // for every north-south pair of nodes, check if a path exists
 vector<int> path;
 for (int i = 0; i<north.size(); i++)
 {
  for (int j=0; j<south.size(); j++)
  {
   path = player1__.find_path(north[i],south[j]);
   if (path.size()>0)
   {
    if (screen)
    {
      cout<<"found north-south path between nodes "<<north[i]<<" "<<south[j]<<" \n";
      cout<<"path: ";
      for (auto k: path)
        cout<<k<<" ";
      cout<<"\n";
    }
    return true;
   }
  }
 } 

 return false; 
}



bool hex_board::check_east_west_path_for_player2(graph player2__, bool screen)
{

 vector<int> east, west;
 vector<int> p2_nodes = player2__.get_nodes();

 for (int i=0; i<p2_nodes.size(); i++)
 { 
  if (p2_nodes[i] % nside == 0)
     west.push_back(p2_nodes[i]);
  else if ((p2_nodes[i]+1) % nside == 0)
     east.push_back(p2_nodes[i]);
 }
 
 if (east.size() <= 0 || west.size() <= 0) {return false;} 

 // for every east-west pair of nodes, check if a path exists
 vector<int> path;
 for (int i = 0; i<east.size(); i++)
 {
  for (int j=0; j<west.size(); j++)
  {
   path = player2__.find_path(east[i],west[j]);
   if (path.size()>0)
   {
    if (screen)
    {
      cout<<"found east-west path between nodes "<<east[i]<<" "<<west[j]<<" \n";
      cout<<"path: ";
      for (auto k: path)
        cout<<k<<" ";
      cout<<"\n";
    }
    return true;
   }
  }
 } 


 return false; 
}



