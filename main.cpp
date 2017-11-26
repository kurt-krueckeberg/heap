/* 
 * File:   main.cpp
 * Author: kurt
 *
 * Created on July 15, 2014, 12:10 PM
 */

#include <cstdlib>
#include <iostream>
#include <cmath>
#include "heap.h"

using namespace std;

int height(int size) 
{
  int log = std::log2(size);  
        
  return log + 1;
  
}


int main(int argc, char** argv) 
{
    min_heap<double> h; // TODO: try min_heap<double> h
    
    for (auto i = 10; i < 120;  i += 10) {
        
        double dble = static_cast<double>(i);
        dble -= 5;
        
        int priority = i;
        
         h.add(priority, dble);
         
         cout << "\n\nPrinting heap"; 
         
          h.print_heap(cout); 
    }
    
    cout << "\n";
    
    cout << h;
    cout << endl;
    
    double d;
    
    while (!h.isEmpty()) {
        
        d = h.peekTop();

        cout << "\nThe top of the heap was: " << d << endl;

        h.remove();

        cout << "Heap is: " <<  h << endl;
    }
     
    
    return 0;
}

