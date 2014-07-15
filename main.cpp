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
    Heap<double> h;
    
    for (auto i = 10; i < 120;  i += 10) {
        
        double dble = static_cast<double>(i);
        
        int priority = i;
        
         h.add(priority, dble);
    }
    
    double d;
    
    while (!h.isEmpty()) {
        
        h.remove(d);
        cout << "The top of the heap was: " << d << endl;
    }
     
    
    return 0;
}

