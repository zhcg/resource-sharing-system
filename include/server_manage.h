/*************************************************************************
	> File Name: server_manage.h
	> Author: 
	> Mail: @163.com 
	> Created Time: 2017年03月23日 星期四 06时33分29秒
 ************************************************************************/

#ifndef _SERVER_MANAGE_H
#define _SERVER_MANAGE_H

#define CLIENT_NUM 21

typedef struct client_status
{
	int client_fd;
	char client_name[16];
	char client_ip[16];
	int client_is_online;
}cli_status_t ;

cli_status_t devlist[CLIENT_NUM];

int client_control_sockfd;
extern int init_control();
extern int print_devlist();
extern int print_reslist();
extern int create_link_manage_thread(void);

#endif
