#ifndef HEAP_H_23974923874
#define HEAP_H_23974923874
/* 
 * File:   heap.h
 * Author: kurt
 *
 * Created on July 15, 2014, 12:10 PM
 */
#include <vector>

template<typename T> class Heap {

  std::vector<T> vec;

 public:
 
    class Node {
      private:
        int priority;
        T   data; 
      public: 
        friend bool operator < (const Node& lhs, const Node& rhs);
 
        Node(int pr, const T& t) : priority(pr), data(t) {}
 
        T getData() const { return data}
        int getPriority() const { return priority; } 
    };
    
    
     Heap(int size);
     bool isEmpty() const;
     int getNumberOfNodes() const;
     int getHeight() const;
     T peekTop() const;
     bool add(const T& t);
     int remove(T& t);
     void clear();
};

inline bool operator < (const Node& lhs, const Node& rhs)
{
   return lhs.priority < rhs.priority;
}

template<typename T> inline Heap<T>::isEmpty() const
{
  return vec.size() == 0;
}

template<typename T> inline T Heap<T>::peekTop() const
{
   if (vec.size() > 0) {

      return vec[0].getData();

   } else {

   }
}

template<typename T> inline Heap<T>::Heap(int size) : vec(size)
{
}

// What if is is empty?
template<typename T> int Heap<T>::remove(T& t)
{
   t = vec[0].getData();
   int priority = vec[0].getPriority();
     
   // put last item in root
   vec[0] = vec.pop_back();

   rebuildHeap(0);

   return priority;
}

template<typename T> inline Heap<T>::add(int x, T)
{
    vec.push_back(Node(x, T)); // use emplace?

    int index = vec.size() - 1;

    bool inPlace = false;

    int parentIndex;
    // move new item up
    while (index >= 0 && !inPlace) {

        parentIndex = (index - 1) / 2;

        if (vec[index] < vec[parent]) {

           inPlace = true;

        }  else {        

           std::swap(vec[index], vec[parentIndex]);

           index = parentIndex;
        }
    } 
}

template<typename T> inline Heap<T>::rebuildHeap(int root)
{
  int child = 2 * root + 1; // index of root's left child

  if (child < vec.size()) {

     // root is not a leaf, so it has a left child at child    
     
     int right_child = child + 1; // index of right child, if any

     if (right_child < vec.size() && vec[right_child] > vec[left_child]) {
            
           child = right_child; // index of larger child  
      }
           
      if (vec[root] < vec[child]) {

           std::swap(vec[root], vec[child]); 

           rebuildHeap(child);
      }  
  }
}
#endif	

