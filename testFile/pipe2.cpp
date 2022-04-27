#include <stdio.h>
#include <sys/types.h>	//mkfifo
#include <sys/stat.h>	//mkfifo
#include <fcntl.h>	//Open
#include <unistd.h>		//write read
#define FIFO "/root/netLibZ/testFile/pipe2Fifo"

using namespace std;

int main()
{
    int fd;
    int pid;

	/*创建命名管道*/
    if((pid = mkfifo(FIFO,0777))==-1) 
    {
		perror("create fifo channel failed!");
		return 1;
    }
    else  printf("create success!\n");

	/*打开命名管道*/
	fd = open(FIFO, O_RDWR);  

    if(fd == -1)
    {
		perror("cannot open the FIFO");
		return 1;
    }

	 /*写入消息*/
	const char buf[100] = "hello world";
    if(write(fd,buf, sizeof(buf)) == -1) 
    {
		perror("write data error!");
		return 1;
    }
    else printf("write data success!\n");

	 /*读取消息*/
	char r_msg[100];
    if(read(fd, r_msg, sizeof(r_msg)) == -1) 
    {
		perror("read error!");
		return 1;
    }
    else printf("the receive data is:  %s\n",r_msg);

	unlink(FIFO);
	/*关闭文件*/
    close(fd);   
    return 0;
}
