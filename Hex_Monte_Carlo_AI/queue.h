#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <vector>


// priority queue 


class queue
{

 public:

 queue();

 ~queue();

 queue(queue&);

 void chgPrioirity(int, int, int);

 void minPriority(int&, int&, int&);

 bool contains(int);
 
 int get_index(int);

 void Insert(int, int, int);

 int get_value(int);
 
 int get_prev(int);

 int size();

 private:

 std::vector<int> element;
 std::vector<int> value;
 std::vector<int> prev;

};

#endif
