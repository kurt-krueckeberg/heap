/* 
 * File:   main.cpp
 * Author: kurt
 *
 * Created on July 15, 2014, 12:10 PM
 */

#include <cstdlib>
#include "heap.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{
    Heap<double> h;
    
    for (auto i = 10; i < 120;  i *= 10) {
        
        double d = static_cast<double>(i);
        
         h.add(i, d);
    }
    
    
    return 0;
}

