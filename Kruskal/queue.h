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

 void chgPrioirity(int, double, int);

 void minPriority(int&, double&, int&);

 bool contains(int);
 
 int get_index(int);

 void Insert(int, double, int);

 double get_value(int);
 
 int get_prev(int);

 int size();

 private:

 std::vector<int> element;
 std::vector<double> value;
 std::vector<int> prev;

};

#endif
