#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED
#include <vector>

struct node{
    int value, low, num, parent;
    bool is_ap;
    node(int n)
    {
        value=n;
        low = num = 0;
        parent=-1;
        is_ap=0;
    }
};

struct graph{
    int vertex_count, counter;
    std::vector<std::vector<int>> adj_list; //first one is index, second one is information if that node was already checked
    std::vector<node> vertex_list;
    std::vector<bool> visited;
    std::vector<bool> checked;
    std::vector<std::pair<int, int>> result; //vector of results

    graph(int n)
    {
        vertex_count=n;
        adj_list.resize(vertex_count);
        visited.resize(vertex_count);
        for(int i=0; i<vertex_count; i++)
        {
            vertex_list.push_back(node(i));
            checked.push_back(0);
        }
    }
    void addEdge(int src, int dest)
    {
        adj_list[src].push_back(dest);
        adj_list[dest].push_back(src);
    }

    int DFSutil(int v, int j)
    {
        visited[v]=true;
        ++j;
        vertex_list[v].num=++counter;
        for(int i=0; i<adj_list[v].size(); i++)
        {
            if(visited[adj_list[v][i]]==false)
            {
                vertex_list[adj_list[v][i]].parent=v;
                j=DFSutil(adj_list[v][i], j);
            }
        }
        return j;
    }

    int DFS(int v)
    {
        for(int i=0; i<vertex_count; ++i) visited[i]=false;
        counter=0;
        int j=DFSutil(v, 0);
        return j;
    }

/*    void assignLowutil(int v)
    {
        int child=0;

        vertex_list[v].low=vertex_list[v].num;

        std::cout<<v<<" "<<vertex_list[v].num<<" "<<vertex_list[v].parent<<std::endl;

        for(int i=0; i<adj_list[v].size(); i++)
        {
            if(vertex_list[adj_list[v][i]].num>vertex_list[v].num)
            {
                child++;
                assignLowutil(adj_list[v][i]);
                if(vertex_list[v].parent==-1) continue; //we are root
                if(vertex_list[adj_list[v][i]].low>=vertex_list[v].num)
                    vertex_list[v].is_ap=1;

                vertex_list[v].low=std::min(vertex_list[v].low, vertex_list[adj_list[v][i]].low);

            }else if(vertex_list[v].parent != vertex_list[adj_list[v][i]].value)
                vertex_list[v].low=std::min(vertex_list[v].low, vertex_list[adj_list[v][i]].num);
        }
        if(vertex_list[v].parent==-1 && child>1) //we are root and have more than 2 child so we are AP
            vertex_list[v].is_ap=1;
    } */

    void assignLowutil(int v)
    {
        int child=0;

        visited[v]=true;
        vertex_list[v].low=vertex_list[v].num;

        for(int i=0; i<adj_list[v].size(); i++)
        {
            if(visited[adj_list[v][i]]==false)
            {
                child++;
                vertex_list[adj_list[v][i]].parent=v;
                assignLowutil(adj_list[v][i]);
                vertex_list[v].low=std::min(vertex_list[v].low, vertex_list[adj_list[v][i]].low);
            }else if(vertex_list[v].parent != vertex_list[adj_list[v][i]].value)
               vertex_list[v].low=std::min(vertex_list[v].low, vertex_list[adj_list[v][i]].num);
            if(vertex_list[adj_list[v][i]].low>=vertex_list[v].num)
                vertex_list[v].is_ap=true;
        }
        if(vertex_list[v].parent==-1 && child>1) //we are root and have more than 2 child so we are AP
            vertex_list[v].is_ap=1;
    }

    void assignLow(int v)
    {
        for(int i=0; i<vertex_count; ++i) visited[i]=false;
        assignLowutil(v);
    }

    bool check_long_bridge(int v, int u)
    {
        for(int i=0; i<visited.size(); ++i) visited[i]=0;
        visited[v]=true;
        visited[u]=true;
        int i=0;
        while(visited[i])
            ++i;
        if(DFSutil(i, 0)==(vertex_count-2))
            return false;
        else return true;
    }

    void ap_long_bridges()
    {
        for(int i=0; i<vertex_list.size(); ++i)
        {
            if(vertex_list[i].is_ap==1)
            {
                for(int j=0; j<adj_list[i].size(); ++j)
                {
                    if(!checked[adj_list[i][j]] && (adj_list[adj_list[i][j]].size()>1 || check_long_bridge(adj_list[i][j], i)))
                    {
                        result.push_back(std::make_pair(i, adj_list[i][j]));
                    }
                }
                checked[i]=1;
            }
        }
    }

    void long_bridges()
    {
        for(int i=0; i<adj_list.size(); ++i)
        {
            if(checked[i]==1) continue;
                for(int j=0; j<adj_list[i].size(); ++j)
                {
                    if(!checked[adj_list[i][j]] && check_long_bridge(adj_list[i][j], i))
                        result.push_back(std::make_pair(i, adj_list[i][j]));
                }
            checked[i]=1;
        }
    }
};

#endif // GRAPH_HPP_INCLUDED
