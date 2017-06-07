//子进程结束时会给父进程发送SIGCHLD信号
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>

void handler(int sig){
        printf("sig is %d,pid is %d\n",sig,getpid());
}

int main()
{
        signal(SIGCHLD,handler);
        int id=fork();
        if(id<0){
                perror("fork");
                exit(-1);
        }
        else if(id==0){
                sleep(3);
                printf("i am child, my pid is %d\n",getpid());
                exit(2);
        }
        else{
                while(1){
                        printf("i am parent,my pid is %d\n",getpid());
                        sleep(1);
                }
        }
        return 0;
}


//进程等待的异步方式
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>

void handler(int sig){
        printf("sig is %d,pid is %d\n",sig,getpid());
}

int main()
{
        int id=fork();
        if(id<0){
                perror("fork");
                exit(-1);
        }
        else if(id==0){
                sleep(3);
                printf("i am child, my pid is %d\n",getpid());
                exit(2);
        }
        else{
                struct sigaction act;
                act.sa_handler = handler;
                sigemptyset(&act.sa_mask);
                act.sa_flags = 0;
                sigaction(SIGCHLD, &act, NULL);
                while(1){
                        printf("i am parent,my pid is %d\n",getpid());
                        sleep(1);
                }
        }
        return 0;
}
