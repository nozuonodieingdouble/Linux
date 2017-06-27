#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

void mydaemon()
{
        umask(0);
        if(fork()>0){
                exit(0);
        }
        setsid();
        chdir("/");
        close(0);
        close(1);
        close(2);
}

int main()
{
        mydaemon();
        while(1);
        return 0;
}