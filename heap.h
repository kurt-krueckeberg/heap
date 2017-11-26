#ifndef HEAP_H_23974923874
#define HEAP_H_23974923874
#include <iosfwd>
#include <ostream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <queue>
#include <exception>
#include <cmath>
#include <utility>
#include <memory>

/*
 * Generic heaps that works as max heap and min heap. i.e., 
 */

template<class T, class Comp=std::less<int> > class heap; 
template<class T> class max_heap : public heap<T>  {}; 
template<class T> class min_heap : public heap<T, std::greater<T>>  {}; 

template<class T, class Comp> class heap {
  
    class Node {
        
        friend class heap<T, Comp>;

        union {
           std::pair<int, T> pair;
           std::pair<const int, T> constkey_pair;
        };
       
        constexpr const std::pair<const int, T>& getPair() const noexcept
        { 
          return constkey_pair; 
        } 

        constexpr std::pair<const int, T>& getPair() noexcept
        { 
          return constkey_pair; 
        }

      public: 
               
        Node(int priority, const T& t) : pair{priority, t} {}

        Node(int priority, T&& t) : pair{priority, std::move(t)} {}

        Node(const Node& n) : pair{n.pair}
        {
        }
        
        Node(Node&& node) : pair{std::move(node.pair)} 
        {
        }

        Node& operator=(const Node& n);
        
        Node& operator=(Node&& n);
         
        const T& getData() const 
        { 
            return pair.second; 
        }
         
        T& getData() noexcept
        { 
            return pair.second; 
        }

        int getPriority() const noexcept 
        { 
            return pair.first; 
        } 
       
        friend bool operator<(const Node& lhs, const Node& rhs)
        { 
            return lhs.getPriority() < rhs.getPriority();
        }
        
        friend bool operator>(const Node& lhs, const Node& rhs)
        {
            return lhs.getPriority() > rhs.getPriority(); 
        }    

        std::ostream& print(std::ostream& ostr) const noexcept
        {          
          return  ostr << '[' << getPriority() << ']' << std::flush;
        } 

        friend std::ostream& operator<<(std::ostream& ostr, const Node& node)
        {
            return node.print(ostr);
        }
    };

    std::vector<Node> vec;
    Comp compare_functor;
    
    int size;

    bool compare(const Node& lhs, const Node& rhs)
    {
       return compare_functor(lhs.getPriority(), rhs.getPriority());		
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

    int parent(int pos) const noexcept
    {
       return (pos - 1) / 2; 
    }

    int leftChild(int pos) const noexcept
    {
       return 2 * pos + 1;
    }

    int rightChild(int pos) const noexcept
    {
       return 2 * pos + 2;
    }

    bool is_leaf(int pos) const noexcept 
    { 
       return leftChild(pos) >= vec.size() ? true : false; 
    }  

    public:   

     using value_type = std::pair<const int, T>; 
     using reference = std::pair<const int, T>&; 
   
     heap(int size);
     heap();
     
     bool isEmpty() const;
     
     T peekTop() const;
     
     void add(int priority, const T& t);
     
     bool remove();
     
     void clear();
     
     int height() const noexcept;
     
     void print_heap(std::ostream&) const noexcept; 

     void show_level(int height, int level, std::ostream& ostr) const noexcept; 

     friend std::ostream&  operator<<(std::ostream&  ostr, const heap& lhs_heap) 
     {
         lhs_heap.print_heap(ostr);
         return ostr;
     }
};

template<class T, class Comp> typename heap<T, Comp>::Node& heap<T, Comp>::Node::operator=(const typename heap<T, Comp>::Node& n)
{
   if (this != &n) { 
    
        pair = n.pair;           
   } 
   return *this;
}       

template<class T, class Comp> typename heap<T, Comp>::Node& heap<T, Comp>::Node::operator=(typename heap<T, Comp>::Node&& n)
{
   if (this != &n) { 

        pair = std::move(n.pair);       
    } 
    return *this;
}       

template<class T, class Comp> inline bool heap<T, Comp>::isEmpty() const
{
  return vec.size() == 0;
}

template<class T, class Comp> T heap<T, Comp>::peekTop() const
{
   if (vec.size() > 0) {

      return vec[0].getData();

   } else {
   
      throw std::logic_error(std::string("peekTop() called on empty heap"));
   }
}

template<class T, class Comp> inline heap<T, Comp>::heap(int sz) : vec(sz), size{0} 
{
}

template<class T, class Comp> inline heap<T, Comp>::heap() : vec(), size{0}
{
}


template<class T, class Comp> void heap<T, Comp>::add(int x, const T& t)
{
    ++size;

    vec.push_back(Node(x, t)); 

    int index = vec.size() - 1;

    swim(index); // repair heap property
}

template<class T, class Comp> void heap<T, Comp>::swim(int index)
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

template<class T, class Comp> bool heap<T, Comp>::remove()
{
   if (vec.empty()) {

       return false;
   }

   --size;
     
   // put last item in root
   vec[0] = vec[vec.size() - 1];
           
   vec.pop_back(); // then remove the formerly last item

   sink(0);  // repair heap property

   return true;
}
/*
 * Move the new root downward until we have a valid heap.
 */
 
template<class T, class Comp> inline void heap<T, Comp>::sink(int root)
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

template<typename T, typename Comp> int  heap<T, Comp>::height() const noexcept
{
   if (size == 0) return 0;

   return static_cast<int>(std::log2(size + 1));  
}

template<typename T, typename Comp> void heap<T, Comp>::print_heap(std::ostream& ostr) const noexcept
{
   if (size == 0) return;

   int tree_height = height(); 
  
   auto level = 0;

   int pos = 0;
  
   while (pos < size) {
 
        int tree_level = static_cast<int>(log2(pos + 1) + 1);

        if (level != tree_level) {

           level = tree_level;

           show_level(tree_height, level, ostr);  
        }  
                
        ostr << vec[pos] << "  ";
        
        ++pos;
   }
}

template<class T, class Comp> void heap<T, Comp>::show_level(int height, int current_level, std::ostream& ostr) const noexcept
{
  ostr << "\n\n" << "current_level = " <<  current_level << ' '; 
     
  // Provide some basic spacing to tree appearance.
  std::size_t num = height - current_level + 1;
  
  std::string str(num, ' ');
  
  ostr << str << std::flush; 
} 

/*
template<class T, class Comp> inline typename heap<T, Comp>::array_iterator heap<T, Comp>::begin() noexcept
{
  return iterator{*this};
}

template<class T, class Comp> inline typename heap<T, Comp>::array_iterator heap<T, Comp>::end() noexcept
{
   return iterator{*this, 0};
}

template<class T, class Comp> inline typename heap<T, Comp>::const_array_iterator heap<T, Comp>::begin() const noexcept
{
  return const_array_iterator{*this};
}

template<class T, class Comp> inline typename heap<T, Comp>::const_array_iterator heap<T, Comp>::end() const noexcept
{
   return const_array_iterator{*this, 0};
}
*/
#endif	
