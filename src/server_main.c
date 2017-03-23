/*************************************************************************
	> File Name: server_main.c
	> Author: 
	> Mail: @163.com 
	> Created Time: 2017年03月23日 星期四 06时11分22秒
 ************************************************************************/

#include <common.h>

extern int init_control();
extern int print_devlist();
extern int print_reslist();
extern int create_link_manage_thread(void);

int show_welcome()
{
	printf("****************************************************************\n");
	printf("*                    resource sharing system                   *\n");
	printf("****************************************************************\n");
//	printf("1. show client list\n");
//	printf("2. show resource list\n");
//	printf("q. quit\n");

	return 0;
}

int main(int argc, char **argv)
{
//	char i;
	
	init_control();
	create_link_manage_thread();

	system("clear");
	show_welcome();
	while(1)
	{
		/*
		scanf("%c", &i);
		if (i == '1')
		{
			system("clear");
			print_devlist();
			scanf("%c", &i);
		}
		else if (i == '2')
		{
			system("clear");
			print_reslist();
		}
		else if (i == 'q')
			return 0;
		*/
		
		sleep(100);
	}

	return 0;
}
