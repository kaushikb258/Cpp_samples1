#include <iostream>
#include <vector>

#include "queue.h"

using namespace std;

// these functions are as suggested in the course 



queue::queue()
{ 
 element.clear();
 value.clear();
 prev.clear();
}

queue::~queue()
{
 element.clear();
 value.clear();
 prev.clear();
}

queue::queue(queue &q)
{
 this->element = q.element;
 this->value = q.value;
 this->prev = q.prev;
}


// get index of node
int queue::get_index(int node)
{
 int index = -1;
 if(this->contains(node))
 { 
  for (int i=0; i<this->size(); i++)
  {
   if (this->element[i] == node)
   {
    index = i;
   }
  }
 }
 return index;
}


// change priority of queue
void queue::chgPrioirity(int node, int v, int prev_node)
{
 if(this->contains(node))
 { 
  value[this->get_index(node)] = v;
  prev[this->get_index(node)] = prev_node;
 }
}


// get value of a node (i.e. how much is the path from beginning node to this node)
int queue::get_value(int node)
{
 int val = -1;
 if (this->contains(node))
 {
  val = value[this->get_index(node)];
 }
 return val;
}


// get the previous node for a given node in the queue
int queue::get_prev(int node)
{
 int prev_node = -1;
 if (this->contains(node))
 {
  prev_node = prev[this->get_index(node)];
 }
 return prev_node;
}


// pop the top element in priority queue
void queue::minPriority(int &top_node, int &top_node_value, int &top_node_prev)
{
 top_node = -1;
 top_node_value = 999999; 
 top_node_prev = -1;
 int ii = -1;

 if (this->size() > 0)
 {
  for (int i=0; i<this->size(); i++)
  {
   if (this->value[i] < top_node_value)
   {
    top_node_value = this->value[i];
    top_node = this->element[i];
    top_node_prev = this->prev[i];
    ii = i;
   }
  } 

  if (ii >= 0)
  {
   this->element.erase(element.begin() + ii);
   this->value.erase(value.begin() + ii);
   this->prev.erase(prev.begin() + ii);   
  }

 }
 else
 {
  cout<<"trying to deque empty priority queue \n";
 }

 
 // idiot check to ensure top_node doesn't exist in element
 for (int i=0; i<this->element.size(); i++)
 {
  if (this->element[i] == top_node) {cout<<"something awry "<<top_node<<" \n";}
 }

}


// check if queue contains node
bool queue::contains(int node)
{
 bool present = false;

 for (int i=0; i<this->size(); i++)
 {
  if (this->element[i] == node)
  {
   present = true;
  }
 }
 return present;
}


// insert entry into queue
void queue::Insert(int node, int v, int prev_node)
{
 element.push_back(node);
 value.push_back(v);
 prev.push_back(prev_node);
}


// get size of queue
int queue::size()
{
 if (this->element.size() != this->value.size())
 {
  cout<<"some bug in queue "<<this->element.size()<<" "<<this->value.size()<<"\n";
 }
 else
 {
  return this->element.size();
 }
}
