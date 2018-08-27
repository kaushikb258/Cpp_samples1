#ifndef CLASS_HEX
#define CLASS_HEX

#include <iostream>

#include "graph.h"
#include "queue.h"



class hex_board
{
 public:

 hex_board(int);
 
 ~hex_board();
 
 void empty_nodes();
 
 int check_node(int, int); 
 
 void occupy_node(int, int, int);
 
 void print();

 bool hex_board_full(); 

 void play_hex(bool);
 
 void set_hex_global();

 bool check_end_of_game();

 void set_player_edges();

 bool check_north_south_path_for_player1();

 bool check_east_west_path_for_player2();


 private:

 int nside;
 std::vector<std::vector<int> > node_occupancy; // 0 for empty, 1 for player1, 2 for player2
 graph player1, player2, hex_global; 

};


#endif
