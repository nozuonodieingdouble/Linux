#include<stdio.h>  
#include<unistd.h>  
#include<signal.h>  
  
void printsig(sigset_t* set){  
        int i=1;  
        for(;i<=31;i++){  
                if(sigismember(set,i)){  
                        printf("1");  
                }else{  
                        printf("0");  
                }  
        }  
        printf("\n");  
}  
  
void handler(int sig)  
{  
    if (sig == SIGINT)  
        printf("recv a sig=%d\n", sig);  
}  
  
int main()  
{  
        sigset_t block,oblock,pending;  
        sigemptyset(&block);    //clear and init  
        sigemptyset(&oblock);  
        sigaddset(&block,2);    //2 signal  
        signal(2, handler);  
        sigprocmask(SIG_BLOCK,&block,&oblock); //set block sigset_t  
        int count=0;  
        while(1){  
                sigpending(&pending);  //get pending  
                printsig(&pending);  
                if(count++>5){  
                        printf("recover sig!\n");  
                        sigprocmask(SIG_SETMASK,&oblock,NULL);  
                }  
                sleep(1);  
        }  
        return 0;  
}