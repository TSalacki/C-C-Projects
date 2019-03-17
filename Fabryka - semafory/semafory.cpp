#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <ctime>

using namespace std;

int pid;
int shmid_buffery, shmid_bufferz, shmid_results, semid;
struct sembuf semb;
int yproducer_count, zproducer_count, robot_count, ywait, zwait, pwait;
int *buffery, *bufferz;
int *results;

union semun{
    int val;                //used for SETVAL
    struct semid_ds *buf;   //used for IPC_STAT, IPC
    ushort *array;          //used for GETALL, SETALL
};

void launch_children(int how_many)
{
    if(pid>0)
    {
        while(how_many>0)
        {
            pid = fork();
            srand(time(NULL)+getpid()); //zeby wartosci byly na pewno losowe, za kazdym razem potrzeba unikalenego seedu dla kazdego forka
            if(pid==0) break;
            --how_many;
        }
    }
}

sembuf * set_semaphore(sembuf &sb, int num, int op, int flg)
{
    sb.sem_num=num;
    sb.sem_op=op;
    sb.sem_flg=flg;
    return &sb;
}

void produce_y()
{
    int sleep_time;
    while(true)
    {
        semop(semid, set_semaphore(semb, 6, -1, 0), 1);
        if(semctl(semid, 5, GETVAL)>0) //gdy wyprodukujemy 10 elementow, to koniec
        {
            semop(semid, set_semaphore(semb, 0, -1, 0), 1); //opuszczamy semafor - bedziemy najpierw czytac czy kolejka nie jest pelna
            if(semctl(semid, 3, GETVAL)<5)                  //jesli kolejka nie jest przepelniona, to mozemy dorzucic nowy element
            {
                buffery[semctl(semid, 3, GETVAL)] = ((rand()%9)+1);
                //cout<<"y produce: "<<semctl(semid, 3, GETVAL)<<" "<<buffery[semctl(semid, 3, GETVAL)]<<endl;
                semop(semid, set_semaphore(semb, 3, 1, 0), 1); //zwiekszamy liczbe elementow w kolejce y
            }
            semop(semid, set_semaphore(semb, 0, 1, 0), 1); //podnosimy semafor - niezaleznie czy dodalismy, czy nie element
            semop(semid, set_semaphore(semb, 6, 1, 0), 1); //podnosimy semafor licznika kolejki wynikow
            sleep_time=((rand()%ywait)+1);
            sleep(sleep_time); //spimy zgodnie z zalozeniami zadania
        }else{
        semop(semid, set_semaphore(semb, 6, 1, 0), 1); //podnosimy semafor licznika kolejki wynikow
        return;
        }
    }
}

void produce_z()
{
    int sleep_time;
    while(true)
    {
        semop(semid, set_semaphore(semb, 6, -1, 0), 1);
        if(semctl(semid, 5, GETVAL)>0) //gdy wyprodukujemy 10 elementow, to koniec
        {
            semop(semid, set_semaphore(semb, 1, -1, 0), 1); //opuszczamy semafor - bedziemy najpierw czytac czy kolejka nie jest pelna
            if(semctl(semid, 4, GETVAL)<5)                  //jesli kolejka nie jest przepelniona, to mozemy dorzucic nowy element
            {
                bufferz[semctl(semid, 4, GETVAL)] = ((rand()%9)+1);
                //cout<<"z produce: "<<semctl(semid, 4, GETVAL)<<" "<<bufferz[semctl(semid, 4, GETVAL)]<<endl;
                semop(semid, set_semaphore(semb, 4, 1, 0), 1); //zwiekszamy liczbe elementow w kolejce y
            }
            semop(semid, set_semaphore(semb, 1, 1, 0), 1); //podnosimy semafor - niezaleznie czy dodalismy, czy nie element
            semop(semid, set_semaphore(semb, 6, 1, 0), 1); //podnosimy semafor licznika kolejki wynikow
            sleep_time=((rand()%zwait)+1);
            sleep(sleep_time); //spimy zgodnie z zalozeniami zadania
        }else{
            semop(semid, set_semaphore(semb, 6, 1, 0), 1); //podnosimy semafor licznika kolejki wynikow
            return;
        }
    }
}

void consume()
{
    int sleep_time;
    while(true)
    {
        semop(semid, set_semaphore(semb, 6, -1, 0), 1);
        if(semctl(semid, 5, GETVAL)>0) //gdy wyprodukujemy 10 elementow, to koniec
        {
            semop(semid, set_semaphore(semb, 0, -1, 0), 1); //opuszczamy semafor kolejki y - bedziemy zaraz sprawdzac druga kolejke
            semop(semid, set_semaphore(semb, 1, -1, 0), 1); //opuszczamy semafor kolejki z - bedziemy sprawdzac czy w obu sa jakies elementy
            if(semctl(semid, 3, GETVAL)>0 && semctl(semid, 4, GETVAL)>0) //jesli w kazdej kolejce jest przynajmniej jeden element, to je laczymy
            {
                int index=(10-semctl(semid, 5, GETVAL));
                results[index]=((buffery[0]*10)+(bufferz[0]));
                //cout<<"consume "<<index<<" "<<buffery[0]<<" "<<bufferz[0]<<" "<<results[index]<<endl;
                for(int i=0; i<4; i++)
                {
                    //cout<<i<<" "<<buffery[i]<<" "<<i+1<<" "<<buffery[i+1]<<"bla"<<endl;
                    buffery[i]=buffery[i+1];
                    bufferz[i]=bufferz[i+1];
                }
                buffery[4] = bufferz[4] = 0;
                semop(semid, set_semaphore(semb, 3, -1, 0), 1); //zmniejszamy licznik elementow kolejki y
                semop(semid, set_semaphore(semb, 4, -1, 0), 1); //zmniejszamy licznik elementow kolejki z
                semop(semid, set_semaphore(semb, 5, -1, 0), 1); //zwiekszamy licznik eleementow kolejki wynikowej
            }
            semop(semid, set_semaphore(semb, 0, 1, 0), 1); //podnosimy semafor kolejki y
            semop(semid, set_semaphore(semb, 1, 1, 0), 1); //podnosimy semafor kolejki z
            semop(semid, set_semaphore(semb, 6, 1, 0), 1); //podnosimy semafor licznika kolejki wynikow
            sleep_time=((rand()%zwait)+1);
            sleep(sleep_time); //spimy zgodnie z zal
        }else{
            semop(semid, set_semaphore(semb, 6, 1, 0), 1); //podnosimy semafor licznika kolejki wynikow
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    pid=getpid();
    key_t key=ftok(argv[0], 1);

    /*sprawdzenie czy podano wystarczajaco wiele zmiennych */
    if(argc !=7)
    {
        cout<<"Too few arguments"<<endl;
        return 0;
    }
    /*przygotowanie licznikow i zmiennyc */
    yproducer_count=atoi(argv[1]);
    zproducer_count=atoi(argv[2]);
    robot_count=atoi(argv[3]);
    ywait=atoi(argv[4]);
    zwait=atoi(argv[5]);
    pwait=atoi(argv[6]);

    /*przydzielenie i sprawdzenie poprawnosci pamieci wspoldzielonej */
    shmid_buffery=shmget(key, 5*sizeof(int), 0666 | IPC_CREAT);
    shmid_bufferz=shmget(key+1, 5*sizeof(int), 0666 | IPC_CREAT);
    shmid_results=shmget(key+2, 10*sizeof(int), 0666 | IPC_CREAT);
    buffery = (int *)shmat(shmid_buffery, 0, 0);
    bufferz = (int *)shmat(shmid_bufferz, 0, 0);
    results = (int *)shmat(shmid_results, 0, 0);
    if(buffery < 0 || bufferz < 0 || results < 0)
    {
        cout<<"error with memory"<<endl;
        return 0;
    }
    for(int i=0; i<5; i++) {buffery[i]=0; bufferz[i]=0;}
    for(int i=0; i<10; i++) {results[i]=0;}
    /*stworzenie semaforow i ustalenie wartosci */
    semid = semget(key, 7, 0666 | IPC_CREAT);
    if(semctl(semid, 0, SETVAL, 1)<0) return 0; //mozna obsluzyc kolejke y
    if(semctl(semid, 1, SETVAL, 1)<0) return 0; //mozna obsluzyc kolejke z
    if(semctl(semid, 2, SETVAL, 1)<0) return 0; //mozna obsluzyc kolejke wynikow
    if(semctl(semid, 3, SETVAL, 0)<0) return 0; //liczba elementow w kolejce y
    if(semctl(semid, 4, SETVAL, 0)<0) return 0; //liczba elementow w kolejce z
    if(semctl(semid, 5, SETVAL, 10)<0) return 0; //liczba elementow w kolejce wynikow
    if(semctl(semid, 6, SETVAL, 1)<0) return 0; //mozna oblsuzyc licznik kolejki wynikow
    /* odpalamy producentow linii Y*/
    if(pid>0)
    {
        launch_children(yproducer_count);
        /*produkujemy tylko jesli jestesmy w dziecku */
        if(pid==0) produce_y();
    }
    /*odpalamy procotentow linii Z*/
    if(pid>0)
    {
        launch_children(zproducer_count);
        /*produkujemy tylko jesli jestesmy w dziecku */
        if(pid==0) produce_z();
    }
    /*odpalamy roboty */
    if(pid>0)
    {
        launch_children(robot_count);
        /*lanczymy elementy tylko jesli jestesmy w dziecku */
        if(pid==0) consume();
    }

    /*oczekiwanie na zakonczenie produkcji */
    if(pid>0)
    {
        for(int j=0; j<(yproducer_count+zproducer_count+robot_count); j++) wait(NULL);
        for(int i=0; i<10; i++)
        {
            cout<<i<<" "<<results[i]<<endl;
        }
    }
    /*zwolnienie pamieci wspoldzielonej i semaforow */
    if(pid>0)
    {
        shmdt(buffery);
        shmdt(bufferz);
        shmdt(results);
        shmctl(shmid_buffery, IPC_RMID, NULL);
        shmctl(shmid_bufferz, IPC_RMID, NULL);
        shmctl(shmid_results, IPC_RMID, NULL);
        semctl(semid, 7, IPC_RMID);
    }
    return 0;
}
