#include <cstddef>
#include <cstdlib>
#include <string>

#include <iostream>
#include <chrono>
#include <ctime>

#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinkedList = aisdi::LinkedList<T>;
template <typename T>
using Vector = aisdi::Vector<T>;

void VectorTest(int how_many)
{
	aisdi::Vector<std::string> collection;
  	std::chrono::time_point<std::chrono::system_clock> start, end;
  	//append
  	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many; ++i)
  	{
  		collection.append("ITEM");
  	}
  	end=std::chrono::system_clock::now();
  	std::chrono::duration<double> elapsed_time=end-start;
  	std::cout<<"Appending "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	
  	//popback
  	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many; ++i)
  	{
  		collection.popLast();
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Popping back "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
	
	//prepend
	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many; ++i)
  	{
  		collection.prepend("ITEM");
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Prepending "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
	
	//popfront
	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many; ++i)
  	{
  		collection.popFirst();
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Popping front "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
	
	//insert in middle
	start=std::chrono::system_clock::now();
  	collection.insert(collection.cbegin(), "ITEM");
  	for(int i=0; i<how_many-1; ++i)
  	{
  		collection.insert(collection.cbegin()+1, "ITEM");
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Inserting inside "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	//iterating whole
  	start=std::chrono::system_clock::now();
  	volatile int x = 0;
  	for(auto it=collection.cbegin(); it!=collection.cend(); ++it)
  	{
		x += (*it).size();
  	}
	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Iterating whole collection took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	//erase whole
  	start=std::chrono::system_clock::now();
  	collection.erase(collection.cbegin(), collection.cend());
	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Erasing whole collection took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	
}

void ListTest(int how_many)
{
	aisdi::LinkedList<std::string> collection;
  	std::chrono::time_point<std::chrono::system_clock> start, end;
  	//append
  	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many; ++i)
  	{
  		collection.append("ITEM");
  	}
  	end=std::chrono::system_clock::now();
  	std::chrono::duration<double> elapsed_time=end-start;
  	std::cout<<"Appending "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	
  	//popback
  	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many; ++i)
  	{
  		collection.popLast();
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Popping back "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
	
	//prepend
	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many; ++i)
  	{
  		collection.prepend("ITEM");
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Prepending "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
	
	//popfront
	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many; ++i)
  	{
  		collection.popFirst();
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Popping front "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
	
	//insert in middle
	start=std::chrono::system_clock::now();
  	collection.insert(collection.cbegin(), "ITEM");
  	for(int i=1; i<how_many; ++i)
  	{
  		collection.insert(collection.cbegin()+1, "ITEM");
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Inserting inside "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	//iterating whole
  	start=std::chrono::system_clock::now();
  	for(auto it=collection.cbegin(); it!=collection.cend(); ++it)
  	{
  	}
	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Iterating whole collection took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	//erase whole
  	start=std::chrono::system_clock::now();
  	collection.erase(collection.cbegin(), collection.cend());
	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Erasing whole collection took: "<<elapsed_time.count()<<" seconds"<<std::endl;

}

}
int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 20000;
  std::cout<<"-------------------------------------------------"<<std::endl;
  std::cout<<"Test for vector:"<<std::endl;
  VectorTest(repeatCount);
  std::cout<<"-------------------------------------------------"<<std::endl;
  std::cout<<"Test for linked list:"<<std::endl;
  ListTest(repeatCount);
  std::cout<<"-------------------------------------------------"<<std::endl;
  return 0;
}
