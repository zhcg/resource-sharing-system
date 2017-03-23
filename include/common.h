/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: @163.com 
	> Created Time: 2017年03月23日 星期四 06时27分11秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#if 0
#define PRINT(format, ...) printf("[%s][%s][-%d-] "format"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define PRINT(format, ...)
#endif

#define MAX(A,B) ((A)>(B))?(A):(B)
#define RES_NUM	1024
#define BUF_LEN	512
#define CONTROL_PORT 6666

typedef struct resource_info
{
	int index;
	char name[16];	
	char type[16];	
	char site[16];
}res_info_t;

res_info_t res_info[RES_NUM];
#endif

