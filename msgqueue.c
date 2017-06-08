//消息队列实现进程之间的通信

//需要调用的接口封装
#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define PATHNAME "."
#define PROJ_ID 0X6666

#define SERVER_TYPE 1 
#define CLIENT_TYPE 2

struct msgbuf{ 
        long mtype;
        char mtext[1024];
};

int createMsgQueue();
int getMsgQueue(); 
int destroyMsgQueue(int msgid);
int sendMsg(int msgid,int type,const char* msg);
int rcvMsg(int msgid,int type,char* out);

#endif

//具体的接口实现
#include"comm.h"

static int commMsgQueue(int flags)
{
        key_t _key=ftok(PATHNAME,PROJ_ID);
        if(_key<0){
                perror("ftok");
                return -1;
        }
        int msgid=msgget(_key,flags);
        if(msgid<0){
                perror("msgget");
                return -2;
        }
        return msgid;
}

int createMsgQueue()
{
        return commMsgQueue(IPC_CREAT|IPC_EXCL|0666);
}

int getMsgQueue()
{
        return commMsgQueue(IPC_CREAT);
}

int destroyMsgQueue(int msgid)
{
        if(msgctl(msgid,IPC_RMID,NULL)<0){
                perror("msgctl");
                return -1;
        }
        return 0;
}


int sendMsg(int msgid,int type,const char* msg)
{
        struct msgbuf _mb;
        _mb.mtype=type;
        strcpy(_mb.mtext,msg);
        if(msgsnd(msgid,&_mb,sizeof(_mb.mtext),0)<0){
                perror("msgsnd");
                return -1;
        }
        return 0;
}

int recvMsg(int msgid,int type,char* out)
{
        struct msgbuf _mb;
        if(msgrcv(msgid,&_mb,sizeof(_mb.mtext),type,0)<0){
                perror("msgrcv");
                return -1;
        }
        strcpy(out,_mb.mtext);
        return 0;
}


//会话双方，分别以client和server来表示

//server.c文件
#include "comm.h"

int main()
{
        int msgid=createMsgQueue();

        char buf[1024];
        while(1){
                rcvMsg(msgid,CLIENT_TYPE,buf);
                printf("client say# %s\n",buf);

                printf("Pleasr Enter# ");
                fflush(stdout);
                ssize_t s=read(0,buf,sizeof(buf)-1);
                if(s>0){
                        buf[s-1]=0;
                        sendMsg(msgid,SERVER_TYPE,buf);
                }
                sleep(1);
        }

        destroyMsgQueue(msgid);
        return 0;
}

//client.c文件
#include "comm.h"

int main()
{
        int msgid=getMsgQueue();

        char buf[1024];
        while(1){
                printf("Pleasr Enter# ");
                fflush(stdout);
                ssize_t s=read(0,buf,sizeof(buf)-1);
                if(s>0){
                        buf[s-1]=0;
                        sendMsg(msgid,CLIENT_TYPE,buf);
                }
                rcvMsg(msgid,SERVER_TYPE,buf);
                printf("server say# %s\n",buf);
        }

        return 0;
}

//Makefile文件
.PHONY:all
all:client server

server:server.c comm.c
        gcc -o $@ $^
client:client.c comm.c
        gcc -o $@ $^
.PHONY:clean
clean:
        rm -f client server
