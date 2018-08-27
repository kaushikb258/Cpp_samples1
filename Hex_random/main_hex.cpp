#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "graph.h"
#include "class_hex.h"

using namespace std;


int main()
{

 const int nside = 11;
 cout<<"number of sides in hex: "<<nside<<"\n";;
 
 hex_board h(nside);
 
 cout<<"now play hex \n";
 
 bool human_entry = false;
 h.play_hex(human_entry);

 cout<<"done! \n";
 
 return 0;
}
