#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "graph.h"
#include "class_hex.h"

using namespace std;


int main()
{

 const int nside = 9;
 cout<<"number of sides in hex: "<<nside<<"\n";;
 
 hex_board h(nside);
 
 cout<<"now play hex \n";
 
 bool ai_hex = true;
 h.play_hex(ai_hex);

 cout<<"done! \n";
 
 return 0;
}
