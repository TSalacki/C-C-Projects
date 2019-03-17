#include <cstddef>
#include <cstdlib>
#include <string>

#include <iostream>
#include <chrono>
#include <ctime>
#include <random>
#include <list>

#include "TreeMap.h"
#include "HashMap.h"


void TreeTest(int how_many, std::list<int> data, std::list<int> which_keys)
{
	aisdi::TreeMap<int, int> collection;
  	std::chrono::time_point<std::chrono::system_clock> start, end;
	//add
  	start=std::chrono::system_clock::now();
  	for(auto it=data.begin(); it!=data.end(); ++it)
  	{
  		collection[*it]=*it;
  	}
  	end=std::chrono::system_clock::now();
  	std::chrono::duration<double> elapsed_time=end-start;
  	std::cout<<"Adding "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	
  	//remove all
  	start=std::chrono::system_clock::now();
  	for(auto it=data.begin(); it!=data.end(); ++it)
  	{
  		collection.remove(*it);
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Removing "<<how_many<<" items from beginning took: "<<elapsed_time.count()<<" seconds"<<std::endl;
	
	//create new
	start=std::chrono::system_clock::now();
  	for(auto it=data.begin(); it!=data.end(); ++it)
  	{
  		collection[*it]=*it;
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Adding "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	
  	//iterate through all
	start=std::chrono::system_clock::now();
  	for(auto it=collection.cbegin(); it!=collection.cend(); it++)
  	{
  		
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Iterating whole tree took: "<<elapsed_time.count()<<" seconds"<<std::endl;

  	//select random elements
	start=std::chrono::system_clock::now();
  	for(auto it=which_keys.begin(); it!=which_keys.end(); ++it)
  	{
  		collection.find(*it);
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Finding random elements: "<<elapsed_time.count()<<" seconds"<<std::endl;

}

void HashTest(int how_many, std::list<int> data, std::list<int> which_keys)
{
	aisdi::HashMap<int, int> collection;
  	std::chrono::time_point<std::chrono::system_clock> start, end;
  	//add
  	start=std::chrono::system_clock::now();
  	for(auto it=data.begin(); it!=data.end(); ++it)
  	{
  		collection[*it]=*it;
  	}
  	end=std::chrono::system_clock::now();
  	std::chrono::duration<double> elapsed_time=end-start;
  	std::cout<<"Adding "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;
  	
  	//remove all
  	start=std::chrono::system_clock::now();
  	for(auto it=data.begin(); it!=data.end(); ++it)
  	{
  		collection.remove(*it);
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Removing "<<how_many<<" items from beginning took: "<<elapsed_time.count()<<" seconds"<<std::endl;

	//create new
	start=std::chrono::system_clock::now();
  	for(auto it=data.begin(); it!=data.end(); ++it)
  	{
  		collection[*it]=*it;
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Adding "<<how_many<<" items took: "<<elapsed_time.count()<<" seconds"<<std::endl;

  	//iterate through all
	start=std::chrono::system_clock::now();
  	for(int i=0; i<how_many*0.2; ++i)
  	{
  		which_keys.push_back((rand()%how_many)+1);
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Iterating whole map took: "<<elapsed_time.count()<<" seconds"<<std::endl;

  	//select random elements
	start=std::chrono::system_clock::now();
  	for(auto it=which_keys.begin(); it!=which_keys.end(); ++it)
  	{
  		collection.find(*it);
  	}
  	end=std::chrono::system_clock::now();
  	elapsed_time=end-start;
  	std::cout<<"Finding random elements: "<<elapsed_time.count()<<" seconds"<<std::endl;
}

int main(int argc, char** argv)
{
	std::srand(time(NULL));
  	const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  	int how_many=(repeatCount+1)/2;
  	std::list<int> which_keys;
  	for(int i=0; i<how_many*0.2; ++i)
  	{
  		which_keys.push_back((rand()%how_many)+1);
  	}
  	std::list<int> data;
  	data.push_back(how_many);
  	int j=2;
  	for(int i=1; i<how_many;)
  	{
  		for(int k=0; k<j; ++k)
  		{
  			data.push_back(how_many*(1+2*k)/j);
  		}
 	 	i=i+j;
  		j=j*2;
 	 }
 	 std::cout<<"Test for tree:"<<std::endl;
  	TreeTest(repeatCount, data, which_keys);
  	std::cout<<"-------------------------------------------------"<<std::endl;
  	std::cout<<"Test for hashmap:"<<std::endl;
  	HashTest(repeatCount, data, which_keys);
  	return 0;
}
