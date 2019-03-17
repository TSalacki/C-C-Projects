#include <iostream>
#include <fstream>
#include "Graph.hpp"

using namespace std;

int main()
{
    int vertexCount;
    std::fstream file1, file2;
    std::string filename;

    cin>>filename;
    file1.open(filename, ios::in);
    file2.open("output.txt", ios::out);

    file1>>vertexCount;

    if(vertexCount >= 0 && vertexCount < 4) //with less than 4 verticles it's impossible to find long bridge
        return 0;

    int u,v,edgeCount=0;
    graph my_graph(vertexCount);

    while(file1 >> v >> u)
    {
        my_graph.addEdge(v, u);
    }
    /*
    for(int i=0; i<vertexCount; i++)
        if(my_graph.vertex_list[i].is_ap==1 )
        {
            std::cout<<i<<" is AP"<<std::endl;
            file2<<i<<" is AP"<<std::endl;
        }
    */
    my_graph.DFS(0);
    my_graph.ap_long_bridges();
    my_graph.long_bridges();
    std::cout<<std::endl;
    for(int i=0; i<my_graph.result.size(); ++i)
    {
       std::cout<<my_graph.result[i].first<<" "<<my_graph.result[i].second<<std::endl;
       file2<<my_graph.result[i].first<<" "<<my_graph.result[i].second<<std::endl;
    }
    file1.close();
    file2.close();
    return 0;
}
