#include <iostream>
#include <vector>
#include "quick_sort.h"

using namespace std;


int partition (vector<double> & v, vector<int> &node1, vector<int> &node2, int low, int high)
{
    double pivot = v[high];  
    double temp;
    int i = (low - 1);  // Index of smaller element
    int k;

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (v[j] <= pivot)
        {
            i++;    // increment index of smaller element
            temp = v[i]; v[i] = v[j]; v[j] = temp;  
            k = node1[i]; node1[i] = node1[j]; node1[j] = k;
            k = node2[i]; node2[i] = node2[j]; node2[j] = k;  
        }
    }
    temp = v[i+1]; v[i+1] = v[high]; v[high] = temp;
    k = node1[i+1]; node1[i+1] = node1[high]; node1[high] = k;
    k = node2[i+1]; node2[i+1] = node2[high]; node2[high] = k; 
    return (i + 1);
}


void quickSort(vector<double> &v, vector<int> &node1, vector<int> &node2, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, v[p] is now
           at right place */
        int pi = partition(v, node1, node2, low, high);

        quickSort(v, node1, node2, low, pi - 1);  // Before pi
        quickSort(v, node1, node2, pi + 1, high); // After pi
    }
}

