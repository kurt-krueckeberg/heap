/* 
 * File:   main.cpp
 * Author: kurt
 *
 * Created on July 15, 2014, 12:10 PM
 */

#include <cstdlib>
#include <iostream>
#include "heap.h"

using namespace std;

int main(int argc, char** argv) 
{
    heap<double> h;
    heap<double> h2; // TODO: try min_heap<double> h2
    
    for (auto i = 10; i < 120;  i += 10) {
        
        double dble = static_cast<double>(i);
        dble -= 5;
        
        int priority = i;
        
         h2.add(priority, dble);
    }
    
    double d;
    
    while (!h2.isEmpty()) {
        
        d = h2.peekTop();
        cout << "The top of the heap was: " << d << endl;
        h2.remove();
        cout << "Heap is: " <<  h2 << endl;
    }
     
    
    return 0;
}

