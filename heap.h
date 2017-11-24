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
           const std::pair<int, T> const_pair;
        };
       
        constexpr const std::pair<int, T>& getPair() const noexcept
        { 
          return const_pair; 
        }

      public: 
               
        Node(int pr, const T& t) : pair{pr, t} {}

        Node(int pr, T&& t) : pair{pr, std::move(t)} {}

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
    
        std::ostream& print(std::ostream& ostr) const noexcept
        {
          return  ostr << "[Priority: " << getPriority() << "; Data: " << getData() << "]";
        } 

        friend std::ostream& operator<<(std::ostream& ostr, const Node& node)
        {
            return node.print(ostr);
        }
    };

    class NodeLevelOrderPrinter {
  
        std::ostream& ostr;
        int current_level;
        int height;
  
        void display_level(std::ostream& ostr, int level) const noexcept
        {
          ostr << "\n\n" << "current_level = " <<  current_level << ' '; 
             
          // Provide some basic spacing to tree appearance.
          std::size_t num = height - current_level + 1;
          
          std::string str( num, ' ');
          
          ostr << str; 
        }
  
       public: 
          
       NodeLevelOrderPrinter (int height_in, std::ostream& ostr_in): height{height_in}, ostr{ostr_in}, current_level{0} {}
  
       NodeLevelOrderPrinter (const NodeLevelOrderPrinter& lhs): height{lhs.height}, ostr{lhs.ostr}, current_level{lhs.current_level} {}
  
       void operator()(const Node *pnode, int level)
       { 
           // Did current_level change?
           if (current_level != level) { 
          
               current_level = level;
          
               display_level(ostr, level);       
           }
  
           std::cout << *pnode << ' ' << std::flush;
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

     using value_type = std::pair<int, T>; // or 'const std::pair<int, T>' or 'std::pair<const int, T>'
   
     heap(int size);
     heap();
     
     bool isEmpty() const;
     
     T peekTop() const;
     
     void add(int priority, const T& t);
     
     bool remove();
     
     void clear();
     
     int height() const noexcept;
     
     std::ostream& printLevelOrder(std::ostream& ostr) const noexcept;
     
     template<typename Functor> void levelOrderTraverse(Functor f) const noexcept;
     
     friend std::ostream&  operator<<(std::ostream&  ostr, const heap& lhs_heap)
     {
         return lhs_heap.printLevelOrder(ostr);
     }
         
};
                                    
template<class T, class Comp> std::ostream& heap<T, Comp>::printLevelOrder(std::ostream& ostr) const noexcept
{
  NodeLevelOrderPrinter tree_printer(height(), ostr);  
  
  levelOrderTraverse(tree_printer);
  
  ostr << std::flush;
}

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

template<class T, class Comp> void heap<T, Comp>::add(int x, const T& t)
{
    ++size;

    vec.push_back(Node(x, t)); 

    int index = vec.size() - 1;

    swim(index); // repair heap property
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

   int log = std::log2(size);  
        
   return log + 1;
}

/*
 * F is a functor whose function call operator takes two parameters: a 'const Node *' and an int indicating the depth of the node from the root, which has depth 1.
 */
template<typename T, typename Comp> template<typename Functor> void heap<T, Comp>::levelOrderTraverse(Functor f) const noexcept
{
   if (size == 0) return;
   
   // pair of: 1. Node pointer and 2. level of tree.
   std::queue<std::pair<const Node*, int>> q; 

   auto level = 1;

   int pos = 0;

   q.push(std::make_pair(&vec[pos], level)); 

   while (!q.empty()) {

        auto [pnode, tree_level] = q.front(); // uses C++17 unpacking

        f(pnode, tree_level); // For example: print out all the keys_values in pnode.
         
        if (!is_leaf(pos)) {

           int left = leftChild(pos);                      
           int right = rightChild(pos);

           q.push(std::make_pair(&vec[left], tree_level + 1));  
           q.push(std::make_pair(&vec[right], tree_level + 1));  
        }
        q.pop(); 
   }
}
#endif	
