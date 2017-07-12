#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

static void usage(const char* proc){
        printf("Usage:%s[local_ip][local_port]\n",proc);
}


int main(int argc,char* argv[])
{
        if(argc!=3){
                usage(argv[0]);
                return 1;
        }

        int sock=socket(AF_INET,SOCK_DGRAM,0);
        if(sock<0){
                perror("socket");
                return 2;
        }

        struct sockaddr_in local;
        local.sin_family=AF_INET;
        local.sin_port=htons(atoi(argv[2]));
        local.sin_addr.s_addr=inet_addr(argv[1]);
        socklen_t len=sizeof(local);
        if(bind(sock,(struct sockaddr*)&local,len)<0){
                perror("bind");
                return 3;
        }

        char buf[1024];
        while(1){
                struct sockaddr_in peer;
                socklen_t len=sizeof(peer);
                ssize_t s = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);
                if(s<0){
                        perror("recvfrom");
                        return 4;
                }
                buf[s]=0;
                printf("[%s:%d]#%s\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port),buf);
                sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&peer,len);
        }

        return 0;
