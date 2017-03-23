/*************************************************************************
	> File Name: server_manage.c
	> Author: 
	> Mail: @163.com 
	> Created Time: 2017年03月23日 星期四 06时31分49秒
 ************************************************************************/

#include "common.h"
#include "server_manage.h"

int init_control()
{
	int sockfd,on,i;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<=0)
	{
		perror("create control socket fail\n");
		exit(-1);
	}
	PRINT("control socket success\n");

	on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(CONTROL_PORT);

	if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
	{
		perror("control bind error");
		exit(-1);
	}
	PRINT("control bind success\n");

	if(listen(sockfd,CLIENT_NUM)<0)
	{
		perror("control listen error\n");
		exit(-1);
	}
	PRINT("control listen success\n");
	client_control_sockfd = sockfd;

	for(i=0;i<CLIENT_NUM;i++)
		devlist[i].client_fd = -1;
	for(i=0;i<RES_NUM;i++)
		res_info[i].index = -1;
	return 0;
}

int print_devlist()
{
	int i,offline = 0;
	printf("****************************************************************\n");
	printf("*                            devlist                           *\n");
	for(i=0;i<CLIENT_NUM;i++)
	{
		if(devlist[i].client_fd == -1)
		{
			offline ++;
			continue;
		}
		printf("* %02d, cfd:%02d , ip:%s ,name:%s *\n",i,devlist[i].client_fd,devlist[i].client_ip,devlist[i].client_name);
	}
	printf("*                   max:%02d,online:%02d,last:%02d                   *\n",CLIENT_NUM,CLIENT_NUM-offline,offline);
	printf("****************************************************************\n");

	return 0;
}

int print_reslist()
{
	int i, available= 0;
	printf("****************************************************************\n");
	printf("*                            reslist                           *\n");
	for(i=0;i<RES_NUM;i++)
	{
		if(res_info[i].index == -1)
		{
			available ++;
			continue;
		}
		printf("* %02d, name:%s, type:%s, site:%s *\n",i,res_info[i].name,res_info[i].type,res_info[i].site);
	}
	printf("*                   max:%02d,own:%02d,last:%02d                   *\n",RES_NUM,RES_NUM-available,available);
	printf("****************************************************************\n");

	return 0;
}


void client_link_accept(void* argv)
{
	PRINT("%s thread start.......\n",__FUNCTION__);
	int clientfd;
	int i;
	fd_set fdset;
	struct timeval tv;
	struct timeval timeout;
	memset(&tv, 0, sizeof(struct timeval));
	memset(&timeout, 0, sizeof(struct timeval));
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	char *new_ip = NULL;
	struct in_addr ia;
	int online_dev_num = 0;

	while (1)
	{

		FD_ZERO(&fdset);
		FD_SET(client_control_sockfd, &fdset);

		tv.tv_sec =  1;
		tv.tv_usec = 0;
		switch(select(client_control_sockfd + 1, &fdset, NULL, NULL,&tv))
		{
			case -1:
			case 0:
				break;
			default:
				if (FD_ISSET(client_control_sockfd, &fdset) > 0)
				{
					clientfd = accept(client_control_sockfd, (struct sockaddr*)&client, &len);
					if (clientfd < 0)
					{
						PRINT("accept failed!\n");
						break;
					}
					PRINT("control accept success\n");
					PRINT("clientfd=%d\n",clientfd);
					ia = client.sin_addr;
					new_ip = inet_ntoa(ia);
					PRINT("client_ip=%s\n",new_ip);
					online_dev_num = 0;
					for(i=0; i<CLIENT_NUM; i++)
					{
						if(devlist[i].client_fd == -1)
							continue;
						if(devlist[i].client_fd >= 0 && strcmp(new_ip,devlist[i].client_ip))
							online_dev_num ++;
					}
					if(online_dev_num == CLIENT_NUM-1)
					{
						PRINT("client limit\n");
						//netWrite(clientfd, "HEADR0010LIMITED000\r\n",22);
						usleep(10*1000);
						close(clientfd);
						clientfd = -1;
						break;
					}

					if(clientfd >= 0)
					{
						for(i=0; i<CLIENT_NUM; i++)
						{
							if(devlist[i].client_fd == -1)
							{
								memset(devlist[i].client_name,0,16);
								memcpy(devlist[i].client_name,new_ip,strlen(new_ip)+1);
								PRINT("default name\n");
					
								devlist[i].client_fd = clientfd;
								memset(devlist[i].client_ip, 0, sizeof(devlist[i].client_ip));
								memcpy(devlist[i].client_ip, new_ip, strlen(new_ip)+1);
								break;
							}
						}
					}
					print_devlist();
				}
		}
	}
}

void client_link_manage(void* argv)
{
	PRINT("%s thread start.......\n",__FUNCTION__);
	int ret=0;
	int i,j;

	fd_set socket_fdset;
	struct timeval tv;
	memset(&tv, 0, sizeof(struct timeval));
	struct resource_info tmp={-1,"","",""};

	while(1)
	{
		int maxfd = -1;
		FD_ZERO(&socket_fdset);
		for(i=0; i<CLIENT_NUM ; i++)
		{
			if(devlist[i].client_fd == -1 )
				continue;
			FD_SET(devlist[i].client_fd, &socket_fdset);
			maxfd = MAX(maxfd, devlist[i].client_fd);
		}

		tv.tv_sec =  1;
		tv.tv_usec = 0;

		switch(select(maxfd+ 1, &socket_fdset, NULL, NULL, &tv))
		{
			case -1:
			case 0:
				break;
			default:
				for(i=0; i<CLIENT_NUM ; i++)
				{
					if(devlist[i].client_fd == -1)
						continue;
					if ( FD_ISSET(devlist[i].client_fd, &socket_fdset) )
					{
						//ret=recv(devlist[i].client_fd,(char*)&tmp,sizeof(struct resource_info),0);
						ret=recv(devlist[i].client_fd,(char*)&tmp,1024,0);

						PRINT("ret=%d, client_fd=%d\n", ret,devlist[i].client_fd);
						if (ret <= 0)
						{
							PRINT("client_fd error, destroy the client\n");
							close(devlist[i].client_fd);
							devlist[i].client_fd = -1;
							memset(devlist[i].client_name, 0, 16);
							memset(devlist[i].client_ip, 0, 16);
							print_devlist();
							break;
						}
						for(j=0;j<RES_NUM;j++)
						{
							if (res_info[j].index == -1)
							{
								memcpy(&res_info[j],&tmp, sizeof(struct resource_info));
								res_info[j].index = j;
								PRINT("j=%d,index=%d, name=%s, type=%s, site=%s\n",j,res_info[j].index, res_info[j].name, res_info[j].type, res_info[j].site);
								print_reslist();
								break;
							}
						}
					}
				}
		}
		usleep(50*1000);
	}
}

int create_link_manage_thread(void)
{
	pthread_t link_accept_pthread;
	pthread_create(&link_accept_pthread, NULL, (void*)client_link_accept, NULL);

	pthread_t link_manage_pthread;
	pthread_create(&link_manage_pthread, NULL, (void*)client_link_manage, NULL);

	return 0;
}
