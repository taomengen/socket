#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 6666 /*port*/

int main(int argc, char* argv[])
{
	struct sockaddr_in serveraddr;
	char sendline[MAXLINE];
	char recvline[MAXLINE];

	memset(&serveraddr, 0, sizeof(serveraddr));
	memset(sendline, 0, sizeof(sendline));
	memset(recvline, 0, sizeof(recvline));

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		printf("Create the socket Failed!\n");
		exit(1);
	}
	else
	{
		printf("Create the socket Success!\n");
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("xx.xx.xx.xx");//设置服务器IP 
	serveraddr.sin_port = htons(SERV_PORT);

	int ret = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (ret == -1)
	{
		printf("Connect to the server Failed!\n");
		exit(2);
	}
	else
	{
		printf("Connect to the server Success!\n");
	}

	//注意！！！
    //fgets会在所读取的内容后面添加'\0'，这样，fgets读取的内容中会包括行末尾的'\n'字符。
    //因此后面send()发送的时候，长度是strlen(sendline)-1
	//while (gets(sendline) != NULL) //这个函数是不安全的，如果无限输入会造成栈空间溢出。
	while (fgets(sendline, MAXLINE, stdin) != NULL)
	{
		send(sockfd, sendline, strlen(sendline)-1, 0);
		ret = recv(sockfd, recvline, MAXLINE, 0);
		if (ret <= 0)
		{
			perror("Receive Data Failed\n");
			exit(3);
		}

		//注意！！！
		//这个函数是将指定的字符串写入文件流中，不包含字符串结尾符'\0'，因此输出之后不会换行。后面还要单独输出换行。
		fputs(recvline, stdout);
		printf("\n\n");

		memset(sendline, 0, sizeof(sendline));
		memset(recvline, 0, sizeof(recvline));
	}
	exit(0);
}
