//�����ܵ�ͨ��

//.server

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>


int main()
{
        umask(0);
        if(mkfifo("./mypipe",0666|S_IFIFO)<0)	//����һ���ܵ���S_IFIFO��ʾ��һ���ܵ��ļ�
	{	
                perror("mkfifo");
                return 1;
        }

        int fd=open("./mypipe",O_RDONLY);	//ͨ���ܵ��ļ�����ͨ�ţ���serverֻ������
        if(fd<0){	
                perror("open");
                return 2;
        }

	char buf[1024];		//�洢Ҫ��ȡ������
        while(1)
        {
		//��fd�н��ж�ȡ���ŵ�buf�У��ڴ���sizeof��buf-1��
                ssize_t s=read(fd,buf,sizeof(buf)-1); 
                if(s>0){
                        buf[s-1]=0;	//��֤�����س�Ҳ����ȥ
                        printf("client say# %s\n",buf);	//��ȡ����client���͵�
                }
		else if(s==0)	//�������0��˵��client�˳��ˣ����Դ�ʱserverҲ�ر�
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
        int fd=open("./mypipe",O_WRONLY);	//client�Զ�д�ķ�ʽ�򿪹ܵ�
        if(fd<0){
                perror("open");
                return 2;
        }

        char buf[1024];
        while(1)
        {
                printf("Please Enter# ");	//����clientд��ˢ�»�����
                fflush(stdout);
                ssize_t s=read(0,buf,sizeof(buf)-1);
                if(s>0){
                        buf[s]=0;
                        write(fd,buf,strlen(buf));  //���ܵ��ļ�д����bufд�����ڴ�дstrlen(buf)
                }
        }

        close(fd);
        return 0;
}


//����server�ļ�����֤�Ƚ��ܵ�������������ʱ�ܵ���û�����ݣ�server��������״̬����xshell������client�����������ݣ��س������л���Linux��ֻ��client��server������Ϣ