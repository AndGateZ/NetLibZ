#include <stdio.h>
#include <unistd.h> //pipe
#include <string.h>
#include <iostream>
#define MAXSIZE 100

using namespace std;

int main()
{
    int fd[2], pid;
	//fd0 读端  fd1 写端
    
    /*创建管道*/
    if(pipe(fd) == -1)
    {
		perror("create pipe failed!");
		return 1;
    }

    /*创建新进程*/
    pid = fork();

    /*子进程*/
    if(pid == 0)
    {
		close(fd[0]);//子进程关闭读端
		const char buf[MAXSIZE] = "Hello Linux!";
		write(fd[1], buf,sizeof(buf)); 	//向文件中写入数据
		printf("child process SEND message!\n");
    }
    else
    {
		close(fd[1]);//父进程关闭写端
		printf("parent process RECEIVE message is:\n");
		char message[MAXSIZE];
		int receiveLen = read(fd[0], message, sizeof(message)); /*读取消息，返回消息长度*/
		//等待
		int a = 0;
		cin>>a;
		cout<<message<<endl;
		//exit(0);
    }
    return 0;
}



//无名管道智能用于父子进程通信
//实质：管道的实质是一个内核缓冲区，进程以先进先出的方式从缓冲区存取数据，管道一端的进程顺序的将数据写入缓冲区，另一端的进程则顺序的读出数据。
// 半双工：数据只能向一个方向流动；需要双向通信时，需要建立起两个管道
// 场景：只能用于父子进程或者兄弟进程之间(具有亲缘关系的进程);
// 数据的读出和写入：一个进程向管道中写的内容被管道另一端的进程读出。写入的内容每次都添加在管道缓冲区的末尾，并且每次都是从缓冲区的头部读出数据。