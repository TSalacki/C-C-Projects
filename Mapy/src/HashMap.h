#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <functional>
#include <iostream>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private:
  struct Node
  {
    Node *previous;
    Node *next;
    value_type data;
    size_type myhash;
    bool empty;
    Node() : previous(nullptr), next(nullptr), data(std::make_pair(key_type{}, mapped_type{})), myhash(0), empty(1)
    {}
    Node(const KeyType& key) : previous(nullptr), next(nullptr), data(std::make_pair(key, mapped_type{})), myhash(0), empty(0)
    {}
    Node(const KeyType& key, size_type myhash) : previous(nullptr), next(nullptr), data(std::make_pair(key, mapped_type{})), myhash(myhash), empty(0)
    {}
    /*Node(const KeyType& key, const ValueType& value) : previous(nullptr), next(nullptr), parent(nullptr), data(std::make_pair(key, value))
    {}*/
  };
  size_type mapSize=0;
  size_type mapCap=65536;
  size_t max_index=0, min_index=65535;
  Node *node_array[65536];
public:
  HashMap()
  {
    for(int i=0; i<65536; i++)
    {
      node_array[i] = nullptr;
    }
  }

  ~HashMap()
  {
    for(int i=0; i<65536; i++)
    {
      if(node_array[i]!=nullptr)
      delete node_array[i];
    }
  }

  HashMap(std::initializer_list<value_type> list) : HashMap()
  {
    for(auto it = list.begin(); it != list.end(); ++it)
    {
         (*this)[it->first]=it->second;
    }
    //(void)list; // disables "unused argument" warning, can be removed when method is implemented.
    //throw std::runtime_error("TODO1");
  }

  HashMap(const HashMap& other) : HashMap()
  {
    for(auto it = other.cbegin(); it != other.cend(); ++it)
    {
         (*this)[it->first]=it->second; 
    } 
    //(void)other;
    //throw std::runtime_error("TODO2");
  }

  HashMap(HashMap&& other)
  {
    for(unsigned int i=0; i<other.mapCap; ++i)
    {
        node_array[i]=other.node_array[i];
        other.node_array[i]=nullptr;
    }
    mapSize=other.mapSize;
    other.mapSize=0;
    max_index=other.max_index;
    min_index=other.min_index;
    other.max_index=0;
    other.min_index=mapCap;
    //(void)other;
    //throw std::runtime_error("TODO3");
  }

  HashMap& operator=(const HashMap& other)
  {
    if(node_array!=other.node_array) //we only do sth if we copy different maps
    {
      for(auto it = other.cbegin(); it != other.cend(); ++it)
      {
           (*this)[it->first]=it->second; 
      } 
    }
    mapSize=other.mapSize;
    max_index=other.max_index;
    min_index=other.min_index;
    return *this;
    //(void)other;
    //throw std::runtime_error("TODO4");
  }

  HashMap& operator=(HashMap&& other)
  {

    for(auto it = cbegin(); it != cend(); ++it) //first we have to clear our current map
    {
        remove(it);
    } 
    for(unsigned int i=0; i<other.mapCap; ++i)
    {
        node_array[i]=other.node_array[i];
        other.node_array[i]=nullptr;
    }
    mapSize=other.mapSize;
    other.mapSize=0;
    max_index=other.max_index;
    min_index=other.min_index;
    other.max_index=0;
    other.min_index=mapCap;
    return *this;
    //(void)other;
    //throw std::runtime_error("TODO5");
  }

  bool isEmpty() const
  {
    return !mapSize;
    //throw std::runtime_error("TODO6");
  }
  size_type hashF(const key_type& key) const
  {
    return std::hash<key_type>{}(key) % mapCap;
  }
  mapped_type& operator[](const key_type& key)
  {
    size_t index = hashF(key);
    if(node_array[index]==nullptr) //if is empty we create new
    {
      node_array[index] = new Node(key, index);
      if(index>max_index) max_index=index;
      if(index<min_index) min_index=index;
      ++mapSize;
      return node_array[index]->data.second;;
    }else
    {
      Node *tmp = node_array[index];
      while(tmp->data.first!=key)
      {
        if(tmp->next) tmp=tmp->next;
      }
      if(tmp->data.first == key) return tmp->data.second;
      tmp->next = new Node(key, index);
      tmp->next->previous=tmp;
      return tmp->next->data.second;
    }
    //(void)key;
    //throw std::runtime_error("TODO7");
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    if(isEmpty()) throw std::out_of_range("Map is empty!");
    size_t index = hashF(key);
    if(node_array[index]==nullptr) //if slot with key's hash doesnt exist we throw
    {
      throw std::out_of_range("There is no such element");
    }else
    {
      Node *tmp=node_array[index];
      while(tmp) //we go through bucket until we find our key
      {
        if(tmp->data.first == key) return tmp->data.second;
        tmp=tmp->next;
      } 
      throw std::out_of_range("There is no such element");
    }
    //(void)key;
    //throw std::runtime_error("TODO8");
  }

  mapped_type& valueOf(const key_type& key)
  {
    if(isEmpty()) throw std::out_of_range("Map is empty!");
    size_t index = hashF(key);
    if(node_array[index]==nullptr) //if slot with key's hash doesnt exist we throw
    {
      throw std::out_of_range("There is no such element");
    }else
    {
      Node *tmp=node_array[index];
      while(tmp) //we go through bucket until we find our key
      {
        if(tmp->data.first == key) return tmp->data.second;
        tmp=tmp->next;
      } 
      throw std::out_of_range("There is no such element");
    }
    //(void)key;
    //throw std::runtime_error("TODO9");
  }

  const_iterator find(const key_type& key) const
  {
    if (isEmpty()) return cend(); //if its empty we return nullptr
    size_t index = hashF(key);
    if(node_array[index]==nullptr) //if slot with key's hash doesnt exist we throw
    {
      return cend();
    }else
    {
      Node *tmp=node_array[index];
      while(tmp) //we go through bucket until we find our key
      {
        if(tmp->data.first == key) return ConstIterator(tmp, this);
        tmp=tmp->next;
      } 
      return ConstIterator(tmp, this); //if we didint find our key we return nullptr
    }
    //(void)key;
    throw std::runtime_error("TODO10");
  }

  iterator find(const key_type& key)
  {
    if (isEmpty()) return end(); //if its empty we return nullptr
    size_t index = hashF(key);
    if(node_array[index]==nullptr) //if slot with key's hash doesnt exist we return nullptr
    {
      return end();
    }else
    {
      Node *tmp=node_array[index];
      while(tmp) //we go through bucket until we find our key
      {
        if(tmp->data.first == key) return Iterator(tmp, this);
        tmp=tmp->next;
      } 
      return Iterator(tmp, this); //if we didint find our key we return nullptr
    }
    //(void)key;
    throw std::runtime_error("TODO11");
  }

  void remove(const key_type& key)
  {
    if(isEmpty()) std::out_of_range("Map is empty!"); //if map is empty we throw
    size_t index = hashF(key);
    if(node_array[index]==nullptr) //if our element does not exist we throw
    {
      throw std::out_of_range("There is no such element!");
    }else
    {
      Node *tmp=node_array[index];
      while(tmp) //we go through bucket until we find our key
      {
        if(tmp->data.first == key) //we found our element
        {
            if(tmp->previous==nullptr) //we are first element in bucket
            {
              if(tmp->next) //if there are more elements in bucket
              {
                tmp->next->previous=nullptr;
                node_array[index]=tmp->next;
                delete tmp;
                return;
              }else //we are alone
              {
                mapSize--;
                delete tmp;
                node_array[index]=nullptr;
                return;
              }
            }
            if(tmp->next) tmp->next->previous=tmp->previous;
            if(tmp->previous) tmp->previous->next=tmp->next;
            delete tmp;
            return;
        }
        tmp=tmp->next;
      }
      throw std::out_of_range("There is no such element!"); //we havn't found element with our key so we throw
    }
    //(void)key;
    //throw std::runtime_error("TODO12");
  }

  void remove(const const_iterator& it)
  {
    remove(it->first);
    //(void)it;
    //throw std::runtime_error("TODO13");
  }

  size_type getSize() const
  {
    return mapSize;
    //throw std::runtime_error("TODO14");
  }

  bool operator==(const HashMap& other) const
  {
    if(mapCap!=other.mapCap) return 0;
    if(mapSize!=other.mapSize) return 0;
    for(auto it=cbegin(), it2=other.cbegin(); it!=cend(); ++it, ++it2)
    {
      if(it->first!=it2->first || it->second!=it2->second) return 0;
    }
    return 1;
    //(void)other;
    //throw std::runtime_error("TODO15");
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    //if(isEmpty()) return Iterator(nullptr, this);
    return Iterator(node_array[min_index], this);
    //throw std::runtime_error("TODO16");
  }

  iterator end()
  {
    return Iterator(nullptr, this);
    //throw std::runtime_error("TODO17");
  }

  const_iterator cbegin() const
  {
    //if(isEmpty()) return ConstIterator(nullptr, this);
    return ConstIterator(node_array[min_index], this);
    //throw std::runtime_error("TODO18");
  }

  const_iterator cend() const
  {
    return ConstIterator(nullptr, this);
    //throw std::runtime_error("TODO19");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
  friend class HashMap<KeyType, ValueType>;
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;
protected:
  Node *ptr;
  HashMap<KeyType, ValueType> const * mapPtr;
public: 
  ConstIterator(Node* ptr = nullptr, HashMap<KeyType, ValueType> const * mapPtr = nullptr)  : ptr(ptr), mapPtr (mapPtr)
  {}

  ConstIterator(const ConstIterator& other) : ConstIterator(other.ptr, other.mapPtr)
  {
    //(void)other;
    //throw std::runtime_error("TODO20");
  }

  ConstIterator& operator++()
  {
    if(ptr==nullptr) throw std::out_of_range("Iterating after end!");
    if(ptr->next) //if next exists it means that we are inside bucket
    {
      ptr=ptr->next;
    }else if(ptr->myhash < mapPtr->max_index) //if next does not exist and we still have more hashes we have to go there
    {
      auto tmp = ptr->myhash+1;
      while(mapPtr->node_array[tmp]==nullptr) ++tmp;
      ptr=mapPtr->node_array[tmp];
    }else  //we are last
    {
      ptr=nullptr;
    }
    return *this;
    //throw std::runtime_error("TODO21");
  }

  ConstIterator operator++(int)
  {
    if(ptr==nullptr) throw std::out_of_range("Iterating after end!");
    auto tmp(*this);
    if(ptr->next) //if next exists it means that we are inside bucket
    {
      ptr=ptr->next;
    }else if(ptr->myhash < mapPtr->max_index) //if next does not exist and we still have more hashes we have to go there
    {
      auto tmp = ptr->myhash+1;
      while(mapPtr->node_array[tmp]==nullptr) ++tmp;
      ptr=mapPtr->node_array[tmp];
    }else  //we are last
    {
      ptr=nullptr;
    }
    return tmp;
    //throw std::runtime_error("TODO22");
  }

  ConstIterator& operator--()
  {
    if(ptr==mapPtr->node_array[mapPtr->min_index]) throw std::out_of_range("Decrementign before begin!"); //if we are first we throw
    if(ptr==nullptr) 
    {
      if(mapPtr->isEmpty()) throw std::out_of_range("Decrementign before begin!"); //we could be potentialy after end, but map is empty
      ptr=mapPtr->node_array[mapPtr->max_index];
    }else if(ptr->next) //if next exists it means that we are inside bucket
    {
      ptr=ptr->next;
    }else if(ptr->myhash > mapPtr->min_index) //if next does not exist and we still have more hashes we have to go there
    {
      auto tmp = ptr->myhash-1;
      while(mapPtr->node_array[tmp]==nullptr) --tmp;
      ptr=mapPtr->node_array[tmp];
    }else  //we are first
    {
      ptr=nullptr;
    }
    return *this;
    //throw std::runtime_error("TODO23");
  }

  ConstIterator operator--(int)
  {
    if(ptr==mapPtr->node_array[mapPtr->min_index]) throw std::out_of_range("Decrementign before begin!"); //if we are first we throw
    auto tmp(*this);
    if(ptr==nullptr) 
    {
      if(mapPtr->isEmpty()) throw std::out_of_range("Decrementign before begin!"); //we could be potentialy after end, but map is empty
      ptr=mapPtr->node_array[mapPtr->max_index];
    }else if(ptr->next) //if next exists it means that we are inside bucket
    {
      ptr=ptr->next;
    }else if(ptr->myhash > mapPtr->min_index) //if next does not exist and we still have more hashes we have to go there
    {
      auto tmp = ptr->myhash-1;
      while(mapPtr->node_array[tmp]==nullptr) --tmp;
      ptr=mapPtr->node_array[tmp];
    }else  //we are first
    {
      ptr=nullptr;
    }
    return tmp;
    //throw std::runtime_error("TODO24");
  }

  reference operator*() const
  { 
    if(ptr==nullptr) throw std::out_of_range("end of map!");
    return ptr->data; 
    //throw std::runtime_error("TODO25");
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return ptr==other.ptr;
    //(void)other;
    //throw std::runtime_error("TODO26");
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator(Node *ptr = nullptr, HashMap<KeyType, ValueType> const * mapPtr = nullptr): const_iterator(ptr, mapPtr)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
