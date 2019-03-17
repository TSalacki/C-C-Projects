#include <iostream>
#include <thread>
#include <queue>
#include "monitor.h"

using namespace std;

int N, K, P, L, W; //(n - l. bolidow), (k - miejsca w alei), (p - priorytet), (l - tankowania), (w - czas jazdy)

class MyMonitor : Monitor{
    Condition ready_enter, ready_exit;
    queue<int> entry_buffer, exit_buffer; //bufer dostepu do alei dostepowej
    int priority;
    int max;
    int Service_count;
    bool Alley_ready;
    int Ready_to_enter;
    int Ready_to_exit;
public:
    MyMonitor(int n)
    {
        priority=P;
        Alley_ready = true;
        max=n;
        Service_count = 0;
        Ready_to_enter = 0;
        Ready_to_exit = 0;
    }
    void access_alley(int bolid_id)
    {
        cout<<"bolid: "<<bolid_id<<" access_alley"<<endl;
        enter();
        cout<<"bolid: "<<bolid_id<<" alley ready: "<<Alley_ready<<" service_count: "<<Service_count<<endl;
        if(Alley_ready && Service_count < K) //jesli jest dostep to wchodzimy i sygnalizujemy ze zajelismy pas serwisowy
        {
            Alley_ready = false;
            leave();
        }else{ //jesli pas jest zajety, albo nie ma wolnych miejsc, to ustawiamy sie w kolejce
            cout<<"bolid: "<<bolid_id<<" we are not ready!"<<endl;
            cout<<"bolid: "<<bolid_id<<" K: "<<K<<" miejsca: "<<Service_count<<endl;
            Ready_to_enter++;
            if(Service_count >= K) { cout<<"!"<<endl; wait(ready_enter);}
            //entry_buffer.push(bolid_id);
            //if(entry_buffer.size()<K) signal(empty);
            leave();
        }
    }

    void go_to_service_box(int bolid_id)
    {
        cout<<"bolid: "<<bolid_id<<" go_to_service_box"<<endl;
        enter();
        Service_count++;
        quit_access_alley(bolid_id);
        leave();
    }

    void quit_access_alley(int bolid_id)
    {
        cout<<"bolid: "<<bolid_id<<" quit_access_alley"<<endl;
        if(Service_count < P){ //priorytet dla wjezdzajacych
            if(Ready_to_enter > 0){
                cout<<"bolid: "<<bolid_id<<" exits1"<<endl;
                Ready_to_enter--;
                //entry_buffer.pop();
                signal(ready_enter);
            }else if(Ready_to_exit > 0){
                cout<<"bolid: "<<bolid_id<<" exits2"<<endl;
                Ready_to_exit--;
                //exit_buffer.pop();
                signal(ready_enter);
            }else{
                cout<<"bolid: "<<bolid_id<<" exits3"<<endl;
                Alley_ready=true;
                signal(ready_enter);
            }
        }else{
            if(Ready_to_exit > 0){
                cout<<"bolid: "<<bolid_id<<" exits4"<<endl;
                Ready_to_exit--;
                //exit_buffer.pop();
                signal(ready_enter);
            }else if(Ready_to_enter > 0){
                cout<<"bolid: "<<bolid_id<<" exits5"<<endl;
                Ready_to_enter--;
                cout<<"ready to enter: "<<Ready_to_enter<<endl;
                //exit_buffer.pop();
                signal(ready_enter);
            }else{
                cout<<"bolid: "<<bolid_id<<" exits6"<<endl;
                Alley_ready=true;
                signal(ready_exit);
            }
        }
    }

    void exit_service_box(int bolid_id)
    {
        cout<<"bolid: "<<bolid_id<<" exit_service_box"<<endl;
        enter();
        cout<<"bolid: "<<bolid_id<<" alley ready to exit: "<<Alley_ready<<endl;
        if(Alley_ready)
        {
            Service_count--;
            Alley_ready = false;
            leave();
        }else{
            cout<<"bolid: "<<bolid_id<<" we are not ready to exit!"<<endl;
            wait(ready_exit);
            Ready_to_exit++;
            //exit_buffer.push(bolid_id);
            //if(exit_buffer.size()<K) signal(empty);
            leave();
        }
    }
    void exit_access_alley(int bolid_id)
    {
        cout<<"bolid: "<<bolid_id<<" exit_access_alley"<<endl;
        enter();
        quit_access_alley(bolid_id);
        leave();
    }
};

MyMonitor Monit(K);

void ride(int i)
{
    cout<<"bolid: "<<i<<" ride"<<endl;
    int sleep_time;
    sleep_time=((rand()%W)+1);
    sleep(sleep_time); //spimy zgodnie z zalozeniami zadania
}

void refuel(int &i, int l)
{
    cout<<"bolid: "<<l<<" refuel"<<endl;
    i++;
    int sleep_time;
    sleep_time=((rand()%W)+1);
    sleep(sleep_time); //spimy zgodnie z zalozeniami zadania
}

void bolid(int bolid_id)
{
    int refuel_count = 0;
    int sleep_time;
    cout<<"bolid: "<<bolid_id<<endl;
    while(refuel_count < L)
    {
        ride(bolid_id);
        Monit.access_alley(bolid_id);
        Monit.go_to_service_box(bolid_id);
        refuel(refuel_count, bolid_id);
        cout<<"bolid: "<<bolid_id<<" fueled times: "<<refuel_count<<endl;
        Monit.exit_service_box(bolid_id);
        Monit.exit_access_alley(bolid_id);
    }
}

int main(int argc, char *argv[])
{
    //pthread_t
    if(argc !=6)
    {
        cout<<"Too few arguments"<<endl;
        return 0;
    }
    N=atoi(argv[1]);
    K=atoi(argv[2]);
    P=atoi(argv[3]);
    L=atoi(argv[4]);
    W=atoi(argv[5]);
    vector<thread> bolids;
    for(int i=0; i<N; ++i) //startujemy bolidy, i bedzie jednoczesnie indeksem bolidu
    {
        cout<<"start!"<<endl;
        bolids.push_back(thread(bolid, i));
    }
    for(auto it=bolids.begin(); it!=bolids.end(); it++) //czekamy az wszystkie bolidy skoncza jezdzic
    {
        it->join();
    }
    return 0;
}
