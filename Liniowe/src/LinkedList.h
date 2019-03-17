#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
private:
	struct Node{
		Type *element;
		Node *previous;
		Node *next;
		Node():element(nullptr), previous(nullptr), next(nullptr) {}
		Node(const Type& elem) : Node()
		{
			element=new Type(elem);
		}
		~Node()
		{
			if(element) delete element;
		}
	};
	Node *head; //pointer to first node of list
	Node *tail; //pointer to last node of list
	size_t listSize;
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  
  LinkedList() : head(nullptr), tail(nullptr), listSize(0)
  {}

  LinkedList(std::initializer_list<Type> l):LinkedList() 
  {
  	for(auto it = l.begin(); it != l.end(); ++it)
  	{
  		append(*it);
  	} 
  }

  LinkedList(const LinkedList& other):LinkedList()
  {
  	for(auto it = other.begin(); it != other.end(); ++it)
  	{
  		append(*it);
  	}
  }

  LinkedList(LinkedList&& other):LinkedList() 
  {
    head=other.head;
    tail=other.tail;

    other.head=nullptr;
    other.tail=nullptr;

    listSize=other.listSize;
    other.listSize=0;
  }

  ~LinkedList()
  {
  	while(!isEmpty())
		erase(begin());	
  }

  LinkedList& operator=(const LinkedList& other)
  {
  	if(head!=other.head)
  	{
    	while (!isEmpty())
    	erase(begin());
  		for(auto it = other.begin(); it != other.end(); ++it)
  		{
  			append(*it);
  		}
  	}
  	return *this;
  }

  LinkedList& operator=(LinkedList&& other) 
  {
    while (!isEmpty())
    erase(begin());

    head=other.head;
    tail=other.tail;

    other.head=NULL;
    other.tail=NULL;

    listSize=other.listSize;
    other.listSize=0;
	  return *this;
  }

  bool isEmpty() const
  {
  	return !listSize;
  }

  size_type getSize() const
  {
  	return listSize;
  }

  void append(const Type& item)
  {
  	Node * NewElement = new Node(item);
  	if(!head) 
  	{
  		head=NewElement; 
  		tail=NewElement;
  		NewElement->previous=nullptr;
  		NewElement->next=nullptr;
  	}
  	else
  	{
  		NewElement->previous=tail;
  		tail->next=NewElement;
  		tail = NewElement;
    }
    ++listSize;
  }

  void prepend(const Type& item)
  {
  	Node * NewElement = new Node(item);
  	if(!head) 
  	{
  		head=NewElement; 
  		tail=NewElement;
  		NewElement->previous=nullptr;
  		NewElement->next=nullptr;
  	}
  	else
  	{
  		NewElement->next=head;
  		head->previous=NewElement;
  		head = NewElement;
    }
    ++listSize;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
  	if(insertPosition==end()) append(item);
  	else if(insertPosition==begin()) prepend(item);
  	else
  	{
  		Node *tmp = new Node(item);
  		tmp->previous=insertPosition.ptr->previous;
  		tmp->next=insertPosition.ptr;
  		tmp->previous->next=tmp;
  		tmp->next->previous=tmp;
  		++listSize;
  	}
  }

  Type popFirst()
  {
  	if(isEmpty()) std::logic_error("Cant pop empty list");
    const Type ret = *begin();
  	erase(begin());
    return ret;
  }

  Type popLast()
  {
  	if(isEmpty()) throw std::logic_error("Cant pop empty list");
    Type ret;
    ret=*(--end());
  	erase(--end());
    return ret;
  }

  void erase(const const_iterator& possition)
  {
    if(possition.ptr==nullptr) throw std::out_of_range("Cant erase nonexisting element");
  	else
  	{
  		if(head==tail)
  		{
  			delete possition.ptr;
  			head=NULL;
  			tail=NULL;
  			--listSize;
  		}
  		else if(possition.ptr==tail)
  		{
  			tail=tail->previous;
  			tail->next=nullptr;
  			delete possition.ptr;
  			--listSize;
  		}else if(possition.ptr==head)
  		{
  			head=head->next;
  			head->previous=nullptr;
  			delete possition.ptr;
  			--listSize;
  		}else
  		{
  			possition.ptr->next->previous=possition.ptr->previous;
  			possition.ptr->previous->next=possition.ptr->next;
  			delete possition.ptr;
  			--listSize;
  		}
  	}
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
  	if(firstIncluded!=lastExcluded)
  	{
  		auto it=firstIncluded;
  		while(it!=lastExcluded)
  		{
  			erase(it);
  			++it;
  		}
  	}
  }

  iterator begin()
  {
  	return Iterator(head, this);
  }

  iterator end()
  {
  	return Iterator(nullptr, this);
  }

  const_iterator cbegin() const
  {
  	return ConstIterator(head, this);
  }

  const_iterator cend() const
  {
  	return ConstIterator(nullptr, this);
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

template <typename Type>
class LinkedList<Type>::ConstIterator
{
	friend class LinkedList<Type>;
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;
 protected:
	Node *ptr;
	LinkedList<value_type> const * listPtr;
public:
  ConstIterator(Node* ptr = nullptr, LinkedList<value_type> const * listPtr = nullptr)  : ptr(ptr), listPtr (listPtr)
  {}
  
  reference operator*() const
  {
  	if(ptr==nullptr) throw std::out_of_range("end of list!");
  	return *(ptr->element);
  }

  ConstIterator& operator++()
  {
  	if(ptr==nullptr) throw std::out_of_range("Iterating after end!");
  	ptr=ptr->next;
  	return *this;
  }

  ConstIterator operator++(int)
  {
  	if(ptr==nullptr) throw std::out_of_range("Iterating after end!");
  	auto tmp(*this);
  	ptr=ptr->next;
  	return tmp;
  }

  ConstIterator& operator--()
  {
  	if(ptr==listPtr->head) 
  	{
  		throw std::out_of_range("Decrementign before begin!");
  	}
  	if(ptr==nullptr)
  	{
    	ptr=listPtr->tail;
  		return *this; 
    }
    ptr=ptr->previous;
    return *this;
  }

  ConstIterator operator--(int)
  {
  	if(ptr==listPtr->head)
  	{
  		throw std::out_of_range("Decrementing before begin!");
  	}
  	auto tmp= *this;
  	ptr=ptr->previous;
  	return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
  	auto tmp = *this;
  	while(d--)   
  		++tmp;
  	return tmp; 
  }

  ConstIterator operator-(difference_type d) const
  {
  	iterator tmp = *this;
  	while(d--)   
  		--tmp;
  	return tmp; 
  }

  bool operator==(const ConstIterator& other) const
  {
  	return ptr==other.ptr;
  }

  bool operator!=(const ConstIterator& other) const
  {
  	return ptr!=other.ptr;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

explicit Iterator(Node *ptr = nullptr, LinkedList<value_type> const * listPtr = nullptr): const_iterator(ptr, listPtr)
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

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
