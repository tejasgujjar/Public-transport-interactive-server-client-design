	//gcc servers.c -o servers  `mysql_config --cflags --libs` -pthread
	
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <errno.h>
	#include <string.h>
	#include <sys/types.h>
	#include <pthread.h>
	#include "utils.h"
	
	#define PORTNUM1 2343
	
	int main(int argc, char* argv[])
	{
		int opt;
		int portno1;
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	if (strcmp(argv[1],"SJ")==0){
		printf("Starting San Jose Server\n");
		portno1 = 20000;
	}
	else if (strcmp(argv[1],"SF")==0){
		printf("Starting San Francisco Server\n");
		portno1 = 20001;
	}
	
		pipe(fd);
		
		printf("Enter Option 0=Stand Alone Server, 1= Master - Redundant Server:\n");
		scanf("%d",&opt);
		
		if(opt==0 )
			mainServer(argv[1]);
		else if(opt ==1)
		{
		if (fork()==0)
		{
			
			checkStatus(portno1);
		}
		else 
		{
			close (fd[WRITE]); /* Close unused end */
			while(strcmp(status,"redunt")==0 || strcmp(status,"init")==0)
			{	
				bytesRead = read (fd[READ], status, sizeof(status));
			}
			if (strcmp(status,"master")==0) 
			{
				mainServer(argv[1]);
			}	
	  	}
		}
		else{
			printf("invalid\n");
		}
		return 0;
	}
	
