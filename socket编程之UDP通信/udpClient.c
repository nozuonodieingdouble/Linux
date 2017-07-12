#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>


static void usage(const char* proc){
        printf("Usage:%s[server_ip][server_port]\n",proc);
}

int main(int argc,char* argv[])
{
        if(argc!=3){
                usage(argv[0]);
                return 1;
        }

        int sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock<0){
                perror("socket");
                return 2;
        }

        struct sockaddr_in peer;
        peer.sin_family = AF_INET;
        peer.sin_port = htons(atoi(argv[2]));
        peer.sin_addr.s_addr = inet_addr(argv[1]);
        char buf[1024];
        while (1)
        {
                printf("Please Enter#");
                fflush(stdout);
                ssize_t s = read(0, buf, sizeof(buf)-1);
                if (s > 0)
                {
                        buf[s - 1] = 0;
                        sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&peer, sizeof(peer));
                }

                struct sockaddr_in tmp;
                socklen_t len = sizeof(tmp);
                s = recvfrom(sock, buf, sizeof(buf)-1, 0, \
                        (struct sockaddr*)&tmp, &len);
                if (s > 0)
                {
                        buf[s] = 0;
                        printf("server#%s\n", buf);
                }
        }

        return 0;
}
