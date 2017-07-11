#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>

static void usage(const char* proc)
{
        printf("Usage %s[local_ip][local_port]\n", proc);
}


int startup(const char* ip, int port)
{
        int sock = socket(AF_INET, SOCK_STREAM, 0);             //先创建
        if (sock < 0)
        {
                perror("sock");
                return 2;
        }

        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = inet_addr(ip);

        if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
        {
                perror("bind");
                return 3;
        }

        if (listen(sock, 10) < 0)
                {
                perror("listen");
                return 4;
        }

        return sock;
}


int main(int argc,char* argv[])
{
	if (argc != 3)
	{
		usage(argv[0]);
		return 1;
	}

	//经过创建、绑定、监听的套接字称为监听套接字
	int listen_sock = startup(argv[1], atoi(argv[2]));

        while (1)
        {
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int new_fd = accept(listen_sock, (struct sockaddr*)&client, &len);
                if (new_fd < 0)
                {
                        perror("accept");
                        continue;
                }

                printf("get a new client,%s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		while (1)		//单用户单进程
                {
                        char buf[1024];
                        ssize_t s = read(new_fd, buf, sizeof(buf)-1);
                        if (s>0)
                        {
                                buf[s] = 0;
                                printf("client say# %s\n", buf);
                                write(new_fd, buf, strlen(buf));
                        }
                        else
                        {
                                printf("read done...\n");
                                break;
                        }
                }
	}
}