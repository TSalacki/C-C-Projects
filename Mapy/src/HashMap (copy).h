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
    bool empty;
    Node() : previous(nullptr), next(nullptr), data(std::make_pair(key_type{}, mapped_type{})), empty(1)
    {}
    Node(const KeyType& key) : previous(nullptr), next(nullptr), data(std::make_pair(key, mapped_type{})), empty(0)
    {}
    /*Node(const KeyType& key, const ValueType& value) : previous(nullptr), next(nullptr), parent(nullptr), data(std::make_pair(key, value))
    {}*/
  };
  Node *head;
  Node *tail;
  Node *first_elem;
  size_type mapSize;
  size_type mapCap;
  size_t max_index, min_index;
public:
  HashMap(size_type cap=2000) : head(nullptr), tail(nullptr), first_elem(nullptr), mapSize(0), mapCap(cap), max_index(0), min_index(cap)
  {
    head = new Node[cap];
    tail=first_elem=head;
  }

  ~HashMap()
  {
    tail=nullptr;
    mapSize=0;
    mapCap=0;
    max_index=0;
    delete[] head;
    head=nullptr;
  }

  HashMap(std::initializer_list<value_type> list)
  {
    (void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO1");
  }

  HashMap(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO2");
  }

  HashMap(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO3");
  }

  HashMap& operator=(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO4");
  }

  HashMap& operator=(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO5");
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
    //std::string key_str = std::any_cast<std::string>(key);
    size_t index = hashF(key);
    Node *tmp=(head+index);
    int i=0;
    if(tmp->data.first==key && tmp->empty==0) //if we find our item
    {
      return tmp->data.second;
    }else             //if first item is not our, we have to check potential others
    {
      while(tmp->next) 
      {
        if(tmp->data.first==key) 
        {
          return tmp->data.second; //yay! we found our item!
        }
        tmp=tmp->next;
        ++i;
      } //we didn't find our item, so we will create new
      Node *NewElement = new Node(key);
      if(i==0) //if there was nothing, we created new element in hashmap, so we increase number of filled slots
      { 
        ++mapSize;
        tmp = (head+index);
        tmp = NewElement;
        if(index>max_index)
        {
          std::cout<<"new max!"<<std::endl;
          max_index=index;
          tail=(head+index);
          ++tail;
        }
        if(index<min_index)
        {
          std::cout<<"New first!"<<std::endl;
          first_elem=(head+index);
        }
        std::cout<<"before return: "<<(tmp)->data.first<<std::endl;
        return (head+index)->data.second;
      }else
      {
        tmp->next=NewElement;
        NewElement->previous=tmp;
        return tmp->data.second;
      }
    }
    //(void)key;
    //throw std::runtime_error("TODO7");
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO8");
  }

  mapped_type& valueOf(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO9");
  }

  const_iterator find(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO10");
  }

  iterator find(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO11");
  }

  void remove(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO12");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO13");
  }

  size_type getSize() const
  {
    throw std::runtime_error("TODO14");
  }

  bool operator==(const HashMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO15");
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return Iterator(first_elem, this);
    //throw std::runtime_error("TODO16");
  }

  iterator end()
  {
    return Iterator(tail, this);
    //throw std::runtime_error("TODO17");
  }

  const_iterator cbegin() const
  {
    return ConstIterator(first_elem, this);
    //throw std::runtime_error("TODO18");
  }

  const_iterator cend() const
  {
    return ConstIterator(tail, this);
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

  ConstIterator(const ConstIterator& other)
  {
    (void)other;
    throw std::runtime_error("TODO20");
  }

  ConstIterator& operator++()
  {
    if(ptr==nullptr) throw std::out_of_range("Iterating after end!");
    while(ptr->empty && ptr<mapPtr->tail) //we skip empty slots until we hit end
    {
      ++ptr;
    }
    return *this; 
    if(!ptr->next) //there is no element with the same hash
    {
      while(ptr->previous) ptr=ptr->previous; //if there are no next element we have to return to "slot head"
      ++ptr;
    }else //if there are elements with the same hash
    {
      ptr=ptr->next;
    }
    return *this;
    throw std::runtime_error("TODO21");
  }

  ConstIterator operator++(int)
  {
    throw std::runtime_error("TODO22");
  }

  ConstIterator& operator--()
  {
    throw std::runtime_error("TODO23");
  }

  ConstIterator operator--(int)
  {
    throw std::runtime_error("TODO24");
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
