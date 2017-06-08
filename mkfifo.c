//命名管道通信

//.server

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>


int main()
{
        umask(0);
        if(mkfifo("./mypipe",0666|S_IFIFO)<0)	//创建一个管道，S_IFIFO表示是一个管道文件
	{	
                perror("mkfifo");
                return 1;
        }

        int fd=open("./mypipe",O_RDONLY);	//通过管道文件进行通信，让server只读数据
        if(fd<0){	
                perror("open");
                return 2;
        }

	char buf[1024];		//存储要读取的内容
        while(1)
        {
		//从fd中进行读取，放到buf中，期待读sizeof（buf-1）
                ssize_t s=read(fd,buf,sizeof(buf)-1); 
                if(s>0){
                        buf[s-1]=0;	//保证不将回车也读进去
                        printf("client say# %s\n",buf);	//读取的是client发送的
                }
		else if(s==0)	//如果读到0，说明client退出了，所以此时server也关闭
		{	
                        printf("client quit!!!,server quit!!!\n");
                        break;
                }

        }

        return 0;
}


//client
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main()
{
        int fd=open("./mypipe",O_WRONLY);	//client以读写的方式打开管道
        if(fd<0){
                perror("open");
                return 2;
        }

        char buf[1024];
        while(1)
        {
                printf("Please Enter# ");	//提醒client写并刷新缓冲区
                fflush(stdout);
                ssize_t s=read(0,buf,sizeof(buf)-1);
                if(s>0){
                        buf[s]=0;
                        write(fd,buf,strlen(buf));  //往管道文件写，以buf写进，期待写strlen(buf)
                }
        }

        close(fd);
        return 0;
}


//先跑server文件，保证先将管道创建出来，此时管道中没有数据，server处于阻塞状态，在xshell中运行client，并发送数据（回车），切换到Linux，只能client给server发送消息