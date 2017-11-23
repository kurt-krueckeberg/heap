#ifndef HEAP_H_23974923874
#define HEAP_H_23974923874
/* 
 * File:   heap.h
 *
 * Created on July 15, 2014, 12:10 PM
 */
#include <iosfwd>
#include <ostream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <exception>

template<typename T> class Heap; // forward declaration

template<typename T> class Heap {
   
  public:
    class Node; //forward declaration of nested class
    class Node {
        
      friend class Heap<T>;  
      
      private:
        int priority;
        T   data;     
      public: 
                      
        Node(int pr, const T& t) : priority(pr), data(t) {}

        Node(const Node& n) : priority(n.priority), data(n.data)
        {
        }

        Node& operator= (const Node& n);
         
        T getData() const 
        { 
            return data; 
        }
        
        int getPriority() const 
        { 
            return priority; 
        } 
         
        friend bool operator< (const Node& lhs, const Node& rhs)
        { 
            return lhs.getPriority() < rhs.getPriority();
        }
        
        friend bool operator> (const Node& lhs, const Node& rhs)
        {
            return rhs < lhs;
        }    
        
        friend std::ostream& operator << (std::ostream& ostr, const Node& rhs)
        {
            ostr << "[Priority: " << rhs.getPriority() << "; Data: " << rhs.getData() << "]";
            return ostr;
        }
    };
       
    private:
        
        std::vector<Node> vec;
        /* 
         * bottom-up reheapify ("swim up"). Continue to swap the value in index pos with the appropriate value above it, if necessary,
         * until we again have a valid heap. 
         */ 
        void swim(int pos); 
    protected:
     /*
      * top-down reheapify. Move the value in index pos downward ("sink down"), if necessary, until we again 
      * have a valid heap        
      */
        void sink(int pos); 

    public:   
        
     Heap(int size);
     Heap();
     bool isEmpty() const;
     T peekTop() const;
     void add(int priority, const T& t);
     bool remove();
     void clear();
     
     template<typename U> friend std::ostream&  operator<<(std::ostream&  ostr, const Heap<U>& heap);
};

template<typename T> typename Heap<T>::Node& Heap<T>::Node::operator=(const typename Heap<T>::Node& n)
{
   if (this != &n) { 
               
        priority = n.priority;
        data = n.data;
    } 
    return *this;
}       

template<typename T> inline bool Heap<T>::isEmpty() const
{
  return vec.size() == 0;
}

template<typename T> T Heap<T>::peekTop() const
{
   if (vec.size() > 0) {

      return vec[0].getData();

   } else {
   
      throw std::logic_error(std::string("peekTop() called on empty heap"));
   }
}

template<typename T> inline Heap<T>::Heap(int size) : vec(size)
{
}

template<typename T> inline Heap<T>::Heap() : vec()
{
}

template<typename T> bool Heap<T>::remove()
{
   if (vec.empty()) {

       return false;
   }
     
   // put last item in root
   vec[0] = vec[vec.size() - 1];
           
   vec.pop_back(); // then remove the formerly last item

   sink(0);  

   return true;
}

template<typename T> void Heap<T>::swim(int index)
{
    // move new item up until we have a valid heap
    int parentIndex;

    while (index > 0) { 

        parentIndex = (index - 1) / 2;
        
        if (vec[index] < vec[parentIndex]) {
       
           break; 

        }  else {        

           std::swap(vec[index], vec[parentIndex]);

           index = parentIndex;
        }
    }
}

template<typename T> void Heap<T>::add(int x, const T& t)
{
    vec.push_back(Node(x, t)); 

    int index = vec.size() - 1;

    swim(index); // heapify upwards
}

template<typename T> inline void Heap<T>::sink(int root)
{
  int child = 2 * root + 1; // Determine if root is a leaf.

  if (child < vec.size()) {  // If root is not a leaf, get the index of its largest child, either left or right

     int right_child = child + 1; 

     if (vec[right_child] > vec[child]) {
            
           child = right_child; 
      }

      // If root smaller than right child, swap root with larger child...
      if (vec[root] < vec[child]) {

           std::swap(vec[root], vec[child]); 

           // ... and continue the process 
           sink(child);
      }  
  }
}

template<typename U> std::ostream&  operator<<(std::ostream&  ostr, const Heap<U>& heap)
{
    std::ostream_iterator<typename Heap<U>::Node> out_it(ostr, ", ");

    std::copy (heap.vec.begin(), heap.vec.end(), out_it);

    return ostr;
}
#endif	
