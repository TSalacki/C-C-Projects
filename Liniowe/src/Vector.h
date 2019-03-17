#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
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

private:
	size_type vecSize;
	size_type capacity;
	Type *head;
	Type *tail;

	Type* new_size()
	{
		if(vecSize+1<=capacity)
		{
			return nullptr;
		}
		capacity=2*vecSize;
		return new Type[capacity];

	}
	void move_to_left(const const_iterator& pos_to, const const_iterator& pos_from)
    {
        iterator to=iterator(pos_to.ptr, *this);
        const_iterator from = pos_from;
        for(; from != cend(); ++to, ++from)
        {
             *to = *from;
        }
        tail=to.ptr;
    }
    void move_to_right(const const_iterator& position)
    {
    	if(isEmpty())
    	{
    		++tail;
    		return;
    	}
    	auto it=end()-1;
    	++tail;
    	for(; it!=position; --it)
    	{
    		*(it+1)=*it;
    	}
    	*(it+1)=*it;
    }
public:

  Vector(size_type cap=10) : vecSize(0), capacity(cap)
  {
  	head= new Type[cap];
  	tail=head;
  }

  Vector(std::initializer_list<Type> l) : Vector(l.size())
  {
  	for(auto it = l.begin(); it != l.end(); ++it)
  	{
  		append(*it);
	}
  }

  Vector(const Vector& other) : Vector(other.capacity)
  {
  	for(auto it = other.cbegin(); it != other.cend(); ++it)
  	{
  		append(*it);
  	} 
  }

  Vector(Vector&& other)
  {
  	vecSize=other.vecSize;
    capacity=other.capacity;

    head=other.head;
    tail=other.tail;

    other.vecSize=0;
    other.capacity=0;
    other.head=nullptr;
    other.tail=other.head;
  }

  ~Vector()
  {
  	tail=nullptr;
  	vecSize=0;
  	capacity=0;
  	delete[] head;
  	head=nullptr;
  }

  Vector& operator=(const Vector& other)
  {
  	if(head!=other.head && tail!=other.tail)
  	{
    	erase(cbegin(), end());
  		for(auto it = other.cbegin(); it != other.cend(); ++it)
  		{
  			append(*it);
  		}
  	}
  	return *this;
  }

  Vector& operator=(Vector&& other)
  {
  	Type *del=head;
  	vecSize=other.vecSize;
    capacity=other.capacity;

    head=other.head;
    tail=other.tail;

    other.vecSize=0;
    other.capacity=0;
    other.head= nullptr;
    other.tail=other.head;


    delete[] del;
    return *this;
  }

  bool isEmpty() const
  {
  	return !vecSize;
  }

  size_type getSize() const
  {
  	return vecSize;
  }

  void append(const Type& item)
  {
  	Type *tmp;
  	if((tmp=new_size()))
  	{
  		Type *del=head;
  		Type *i;
  		auto it=cbegin();
  		for(i=tmp; it!=end(); ++i, ++it)
  		{

  			*i=*it;
  		}
  		head=tmp;
  		tail=i;
  		delete[] del;
  	}
	++vecSize;
  	*(tail++)=item;
  }

  void prepend(const Type& item)
  {
    Type *tmp;

    if((tmp=new_size()))
      {
        Type *del=head;
        *tmp=item;

        Type *i;
        auto it=begin();
        for(i=tmp+1; it!=cend(); ++i, ++it)
        {
            *i = *it;
        }
        head=tmp;
        tail= i;
        delete[] del;
    }
    else
    {
        move_to_right(cbegin());
        *head=item;
    }
    ++vecSize;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
  	Type *tmp;
  	if(insertPosition==cend()) 
  	{
  		append(item);
		return;
  	}
  	else if (insertPosition==cbegin()) 
  	{
  		prepend(item);
  		return;
  	}
  	else if((tmp=new_size()))
  	{
  		Type *del=head;

  		Type *i;
  		auto it=cbegin();
  		for(i=tmp; it!=insertPosition; ++i, ++it)
  		{
  			*i=*it;
  		}
  		*i=item;
  		for(++i; it!=cend(); ++i, ++it)
  		{

  			*i=*it;
  		}
  		head=tmp;
  		tail=i;
        delete[] del;
  	}
  	else
  	{
  		move_to_right(insertPosition);
  		*((iterator)insertPosition)=item;
  	}
  	++vecSize;
  }

  Type popFirst()
  {
  	if(isEmpty())
  		throw std::out_of_range("Vector cant be popped!");
  	Type elem=*head;
  	move_to_left(cbegin(), cbegin()+1);
  	--vecSize;
  	return elem;
  }

  Type popLast()
  {
  	if(isEmpty())
  		throw std::out_of_range("Vector cant be popped!");
  	--vecSize;
  	return *(--tail);
  }

  void erase(const const_iterator& possition)
  {
  	if(isEmpty())
  	{
  		throw std::out_of_range("Vector cant be erased!");
  	}
  	if(possition==cend())
  	{
  		throw std::out_of_range("Out of range!");
  	}	
  	if(possition==cend()-1)
  	{
  		popLast();
  		return;
  	}
  	move_to_left(possition, possition+1);
  	--vecSize;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
  	if(isEmpty())
        throw std::out_of_range("Vector cant be erased!");
    if(firstIncluded==lastExcluded)
    	return;

    vecSize=vecSize-(lastExcluded.ptr-firstIncluded.ptr);
    move_to_left(firstIncluded, lastExcluded);
  }

  iterator begin()
  {
  	return iterator(head, *this);
  }

  iterator end()
  {
  	return iterator(tail, *this);
  }

  const_iterator cbegin() const
  {
  	return const_iterator(head, *this);
  }

  const_iterator cend() const
  {
  	return const_iterator(tail, *this);
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
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
    
protected:
	Type *ptr;
	const Vector<Type>& vec;
	friend class Vector<Type>;

public:
  explicit ConstIterator(Type* ptr, const Vector<Type>& vec)  : ptr(ptr), vec(vec)
  {}

  reference operator*() const
  {
    if(ptr==vec.tail)
        throw std::out_of_range("Out of range!");
    return *ptr;
  }

  ConstIterator& operator++()
  {
  	if(ptr==vec.tail)
    	throw std::out_of_range("Out of range!");
    ++ptr;
    return *this;
  }

  ConstIterator operator++(int)
  {
  	if(ptr==vec.tail) throw std::out_of_range("Iterating after end!");
  	auto tmp(*this);
  	++ptr;
  	return tmp;
  }

  ConstIterator& operator--()
  {
  	if(ptr==vec.head)
    	throw std::out_of_range("Out of range!");
    --ptr;
    return *this;
  }

  ConstIterator operator--(int)
  {
  	if(ptr==vec.head) throw std::out_of_range("Iterating after end!");
  	auto tmp(*this);
  	--ptr;
  	return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
  	if(ptr+d>vec.tail)
  		throw std::out_of_range("Out of range!");
  	return ConstIterator(ptr+d, vec);
  }

  ConstIterator operator-(difference_type d) const
  {
  	if(ptr-d<vec.head)
  		throw std::out_of_range("Out of range!!!");
  	return ConstIterator(ptr-d, vec);
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
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(Type *ptr, const Vector<Type>& vec): ConstIterator(ptr, vec)
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

#endif // AISDI_LINEAR_VECTOR_H
