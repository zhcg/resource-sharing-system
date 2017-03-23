/*************************************************************************
  > File Name: client.c
  > Author: 
  > Mail: @163.com 
  > Created Time: Mon 18 Jul 2016 10:33:25 AM CST
 ************************************************************************/

#include "include/common.h"

int show_welcome()
{
	printf("****************************************************************\n");
	printf("*                    resource sharing system                   *\n");
	printf("****************************************************************\n");
	printf("Please input resource info:\n");

	return 0;
}

int main(int argc, char** argv)
{
	char name[16]={"aaa"};
	char type[16]={"bbb"};
	char site[16]={"ccc"};

	int    sockfd, n;
	struct sockaddr_in    servaddr;

	if( argc != 2){
		printf("usage: ./client <ipaddress>\n");
		exit(0);
	}

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6666);
	if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
		printf("inet_pton error for %s\n",argv[1]);
		exit(0);
	}

	if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}
#if 0
	printf("send msg to server: \n");
	//fgets(sendline, 4096, stdin);
	if( send(sockfd, sendline, len+7, 0) < 0)
	{
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	if( send(sockfd, sendline, len+7, 0) < 0)
	if( send(sockfd, sendline, len+7, 0) < 0)
#endif

	show_welcome();
	
	while(1)
	{
		printf("name= ");
		scanf("%s",name);
		printf("type= ");
		scanf("%s",type);
		printf("site= ");
		scanf("%s",site);
		sprintf(res_info[0].name,"%s",name);
		sprintf(res_info[0].type,"%s",type);
		sprintf(res_info[0].site,"%s",site);
		//send(sockfd, (char*)&res_info[0], sizeof(res_info), 0);
		send(sockfd, (char*)&res_info[0], 1024, 0);
		printf("upload res info ok\n");

	//	sleep(100);
	}
	close(sockfd);

	return 0;
}

