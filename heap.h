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
    class Node; //forward declaration;
    class Node {
        
      friend class Heap<T>;  
      
      private:
        int priority;
        T   data; 
      public: 
                      
        Node(int pr, const T& t) : priority(pr), data(t) {}

        Node(const Node& n)
        {
           priority = n.priority;
           data = n.data;
        }

        Node& operator= (const Node& n)
        {
           if (this != &n) { 
               
             priority = n.priority;
             data = n.data;
             
           } 
           return *this;
        }
 
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
         void swim(int pos); // bottom-up reheapify ("swim up") 
    protected:
     
        void sink(int pos); // top-down reheapify ("sink down")        

    public:   
     Heap(int size);
     Heap();
     bool isEmpty() const;
     T peekTop() const throw(std::logic_error);
     void add(int priority, const T& t);
     bool remove();
     void clear();
     
     template<typename U> friend std::ostream&  operator<<(std::ostream&  ostr, const Heap<U>& heap);
};
       

template<typename T> inline bool Heap<T>::isEmpty() const
{
  return vec.size() == 0;
}

template<typename T> T Heap<T>::peekTop() const throw(std::logic_error)
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
// What if is is empty?
template<typename T> bool Heap<T>::remove()
{
   if (vec.empty()) {
       return false;
   }
   //t = vec[0].getData();
   //int priority = vec[0].getPriority();
     
   // put last item in root
   vec[0] = vec[vec.size() - 1];
           
   vec.pop_back();

   sink(0);

   return true;
}

template<typename T> void Heap<T>::swim(int index)
{
    // move new item up
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
  int child = 2 * root + 1; // index of root's left child

  if (child < vec.size()) {

     // root is not a leaf, so it has a left child at child    
     
     int right_child = child + 1; // index of right child, if any

     if (right_child < vec.size() && vec[right_child] > vec[child]) {
            
           child = right_child; // index of larger child  
      }
           
      if (vec[root] < vec[child]) {

           std::swap(vec[root], vec[child]); 

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

