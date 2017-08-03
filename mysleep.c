#include<stdio.h>  
#include<unistd.h>  
#include<unistd.h>  
  
void handler(int sig){  
        printf("my sig is %d\n",sig);  
}  
int mysleep(int time){  
        struct sigaction new,old;  
        int usleep=0;  
        new.sa_handler=handler;  
        sigemptyset(&new.sa_mask);  
        new.sa_flags=0;  
        sigaction(SIGALRM,&new,&old);  
        alarm(time);  
        pause();  
        usleep=alarm(0);  
        sigaction(SIGALRM,&new,NULL);  
        return usleep;  
}  
  
int main()  
{  
        while(1){  
                mysleep(3);  
                printf("sleep over\n");  
        }  
        return 0;  
}