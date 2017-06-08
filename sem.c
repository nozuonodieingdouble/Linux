//信号量集实现进程间通信

//comm.h文件

#ifndef _COMM_H
#define _COMM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PATHNAME "."
#define PROJ_ID 0X6666

union semun {
        int val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

int createSemSet(int nums);
int initSemSet(int semid,int which, int val);
int getSemSet();
int destroySemSet(int semid);
int P(int semid,int num);
int V(int semid,int num);

#endif


//com.c文件

#include "comm.h"

static int commSemSet(int nums,int flags)
{
        key_t _k=ftok(PATHNAME,PROJ_ID);
        if(_k<0){
                perror("ftok");
                return -1;
        }

        int semid=semget(_k,nums,flags);
        if(semid<0){
                perror("semget");
                return -2;
        }
        return semid;
}

int createSemSet(int nums)
{
        return commSemSet(nums,IPC_CREAT|IPC_EXCL|0666);
}


int initSemSet(int semid,int which, int val)
{
        union semun _un;
        _un.val=val;
        if(semctl(semid, which, SETVAL, _un) < 0){
                perror("semctl");
                return -1;
        }
        return 0;
}

int getSemSet(int nums)
{
        return commSemSet(nums,IPC_CREAT);
}

int destroySemSet(int semid)
{
        if(semctl(semid,0,IPC_RMID)<0){
                perror("semctl");
                return -1;
        }
        return 0;
}

static int commPV(int semid,int num,int op)
{
        struct sembuf _sf;
        _sf.sem_num=num;
        _sf.sem_op=op;
        _sf.sem_flg=0;

        if(semop(semid,&_sf,1)!=0){
                perror("semop");
                return -1;
        }
        return 0;
}

int P(int semid,int num)
{
        return commPV(semid,num,-1);
}

int V(int semid,int num)
{
        return commPV(semid,num,1);
}


//test_sem测试文件

#include "comm.h"

int main()
{
        int semid=createSemSet(1);
        initSemSet(semid,0,1);

        if(fork()==0){//child
                int _semid = getSemSet(0);
                while(1){
                        P(_semid,0);
                        printf("A");
                        fflush(stdout);
                        sleep(2);
                        printf("A");
                        fflush(stdout);
                        sleep(1);
                        V(_semid,0);
                }
        }
        else{
                while(1){
                        P(semid,0);
                        printf("B");
                        fflush(stdout);
                        sleep(3);
                        printf("B");
                        fflush(stdout);
                        sleep(2);
                        V(semid,0);
                }
        }

        destroySemSet(semid);
        return 0;
}
