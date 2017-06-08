//�����ڴ�ʵ�ֽ��̼�ͨ��
//clientȥд	serverȥ��

//comm.h�ӿ�ͷ�ļ�
#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "."
#define PROJ_ID 0X6666

int createShm(int size);
int getShm(int size);
int destroyShm(int shmid);

#endif


//comm.c����Ľӿ�ʵ��
#include "comm.h"

static int commShm(int size,int flags)
{
        key_t _key = ftok(PATHNAME, PROJ_ID);
        if(_key < 0){
                perror("ftok");
                return -1;
        }
        int shmid = shmget(_key, size, flags);
        if(shmid < 0){
                perror("shmget");
                return -2;
        }
        return shmid;
}

int createShm(int size)
{
        return commShm(size,IPC_CREAT|IPC_EXCL|0666);
}

int getShm(int size)
{
        return commShm(size,IPC_CREAT);
}

int destroyShm(int shmid)
{
        if(shmctl(shmid, IPC_RMID, NULL) < 0){
                perror("shmctl");
                return -1;
        }
        return 0;
}

//�Ự˫����client��server����ʾ

//server.c�ļ�
#include "comm.h"

int main()
{
        int shmid = createShm(4097);
        sleep(2);
        char* addr = shmat(shmid, NULL, 0);
        sleep(3);
        //shmdt(addr);
        sleep(1);
        printf("proc done\n");
        return 0;
}

//client.c�ļ�
#include "comm.h"

int main()
{
        int shmid = getShm(0);
        char* addr = (char*)shmat(shmid, NULL, 0);

        int i=0;
        for(; i<26; ){
                addr[i]='A'+i;
                i++;
                addr[i]=0;
                sleep(1);
        }

        shmdt(addr);
        printf("proc done\n");
        return 0;
}


//Makefile�ļ�
.PHONY:all
all:client server

client:comm.c client.c
        gcc -o $@ $^
server:comm.c server.c
        gcc -o $@ $^
.PHONY:clean
clean:
        rm -f server client
