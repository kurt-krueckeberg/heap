#ifndef HEAP_H_23974923874
#define HEAP_H_23974923874
#include <iosfwd>
#include <ostream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <exception>
/*
 * Generic heaps that works as max heap and min heap. i.e., 
 * using max_heap = Heap<double> // Heap<double, less<int>>
 * using min_heap = Heap<double, greater<int>>
 */
template<class T, class Comp=std::less<int> > class Heap {
  
    class Node {
        friend class Heap<T, Comp>;
        int priority;
        T   data;     
      public: 
        Node(int pr, const T& t) : priority(pr), data(t) {}

        Node(const Node& n) : priority(n.priority), data(n.data)
        {
        }
        
        Node(Node&& node) : priority(std::move(node.priority)), data(std::move(node.data)) 
        {
        }

        Node& operator=(const Node& n);
        Node& operator=(Node&& n);
         
        const T& getData() const 
        { 
            return data; 
        }
         
        T& getData() 
        { 
            return data; 
        }

        int getPriority() const 
        { 
            return priority; 
        } 
    
        friend std::ostream& operator<<(std::ostream& ostr, const Node& rhs)
        {
            ostr << "[Priority: " << rhs.getPriority() << "; Data: " << rhs.getData() << "]";
            return ostr;
        }
    };

    std::vector<Node> vec;
    Comp compare_functor;

    bool compare(const Node& lhs, const Node& rhs)
    {
       return compare_functor(lhs.priority, rhs.priority);		
    }	
    /* 
     * bottom-up repair of heap property ("swim up"). Continues to swap the value in index pos with the parent until parent is not less than vec[pos].
     * until we again have a valid heap. 
     */ 
    void swim(int pos); 
    
    /*
     * top-down reheapify. Move the value in index pos downward ("sink down"), if necessary, until we again 
     * have a valid heap        
     */
    void sink(int pos); 

    bool is_leaf(int pos) const noexcept 
    { 
       int left_child = 2 * pos + 1;
       return left_child >= vec.size() ? true : false; 
    }  

    public:   
        
     Heap(int size);
     Heap();
     bool isEmpty() const;
     T peekTop() const;
     void add(int priority, const T& t);
     bool remove();
     void clear();
    
    friend std::ostream&  operator<<(std::ostream&  ostr, const Heap& heap)  
    {
     std::ostream_iterator<typename Heap::Node> out_it(ostr, ", ");

     std::copy(heap.vec.begin(), heap.vec.end(), out_it);

     return ostr;
    }
};

template<class T, class Comp> typename Heap<T, Comp>::Node& Heap<T, Comp>::Node::operator=(const typename Heap<T, Comp>::Node& n)
{
   if (this != &n) { 
               
        priority = n.priority;
        data = n.data;
    } 
    return *this;
}       

template<class T, class Comp> typename Heap<T, Comp>::Node& Heap<T, Comp>::Node::operator=(typename Heap<T, Comp>::Node&& n)
{
   if (this != &n) { 
               
        priority = std::move(n.priority);
        data = std::move(n.data);
    } 
    return *this;
}       

template<class T, class Comp> inline bool Heap<T, Comp>::isEmpty() const
{
  return vec.size() == 0;
}

template<class T, class Comp> T Heap<T, Comp>::peekTop() const
{
   if (vec.size() > 0) {

      return vec[0].getData();

   } else {
   
      throw std::logic_error(std::string("peekTop() called on empty heap"));
   }
}

template<class T, class Comp> inline Heap<T, Comp>::Heap(int size) : vec(size)
{
}

template<class T, class Comp> inline Heap<T, Comp>::Heap() : vec()
{
}

template<class T, class Comp> bool Heap<T, Comp>::remove()
{
   if (vec.empty()) {

       return false;
   }
     
   // put last item in root
   vec[0] = vec[vec.size() - 1];
           
   vec.pop_back(); // then remove the formerly last item

   sink(0);  // repair heap property

   return true;
}

template<class T, class Comp> void Heap<T, Comp>::swim(int index)
{
    // Move new item up until we have a valid heap
    int parentIndex;

    while (index > 0) { 

        parentIndex = (index - 1) / 2;
        
       if (compare(vec[index], vec[parentIndex])) {
       
           break; 

        }  else {        

           std::swap(vec[index], vec[parentIndex]);

           index = parentIndex;
        }
    }
}
template<class T, class Comp> void Heap<T, Comp>::add(int x, const T& t)
{
    vec.push_back(Node(x, t)); 

    int index = vec.size() - 1;

    swim(index); // repair heap property
}

/*
 * Move the new root downward until we have a valid heap.
 */
 
template<class T, class Comp> inline void Heap<T, Comp>::sink(int root)
{
  int child = 2 * root + 1; // Determine if root is a leaf.

  if (child < vec.size()) {  // If root is not a leaf, get the index of its largest child, either left or right

     int right_child = child + 1; 

     if (compare(vec[child], vec[right_child])) {
            
           child = right_child; 
      }

      // If root smaller than right child, swap root with larger child...
      if (compare(vec[root], vec[child])) {

           std::swap(vec[root], vec[child]); 

           // ... and continue the process 
           sink(child);
      }  
  }
}
#endif	
