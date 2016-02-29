	//gcc -pthread -o servers $(mysql_config --cflags) servers.c $(mysql_config --libs)
	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/types.h>
	#include <errno.h>
	#include <strings.h>
	#include <fcntl.h> 
	#include <unistd.h> 
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <string.h>
	#include <sys/select.h>
	#include <sys/time.h>
	#include <pthread.h>
	#include <mysql.h>
	#include <netdb.h>
	
	#define PORTNUM1 2343
	
	#define READ 0 /* The index of the read end of the pipe */
	
	#define WRITE 1 /* The index of the write end of the pipe */
	
	
	#define RECEIVE_BUFF_LEN 128
	#define SEND_BUFF_LEN 128
	#define SERVICE_PORT_NO 5004
	#define QLEN 15
	#define FIRST_RECEIVE_MSG "PING"
	#define PROMPT_USERNAME "Enter Username: "
	#define PROMPT_CREATE_USERNAME "Would you like to create an account?"
	#define PROMPT_PASSWORD "Enter Password: "
	#define NOTIFY_WRONG_PASSWORD "Incorrect password provided. Terminating Connection."
	#define NOTIFY_ACC_CREATION "Your account has been created"
	#define NOTIFY_INVALID_FILESIZE "Invalid Filesize"
	#define NOTIFY_SENDFILE "Sendfile"
	#define NOTIFY_CORRECT_PASSWORD "You are logged in."
	#define NOTIFY_FILE_NOT_FOUND "The requested file is not found."
	#define DELIMITER '|'
	#define maxFileSize 30000
	#define COUNTER_NAME "next_user_ID"
	
	typedef struct threadData {
			int sock;
			struct  sockaddr_in cli_addr;
			char *buffer;		
		} threadData;
	
	pthread_mutex_t mutexlock;
		
			
	char currentCity[3];
	void *tcpClientHandler(void *tcp);	
	char sRedunt[] = "redunt";
	char sMaster[]= "master";
	char sInit[]="init";
	char status[20] = "init"; 		/*making status as initial at the begining  status init : initial*/
	int beatMissCount =0;
	int hbNotRecieved;
	int hbSent =0;
	int hbReply =0;
	int fd [2], bytesRead; 			/*for pipe read/write for interprocess communication*/
	void communicate(char status[20])
	{
		close(fd[READ]); 			/* Close unused end */
	    write (fd[WRITE],status, strlen (status) + 1); /* include NULL*/
	    close (fd[WRITE]); 			/* Close used end*/
		return;
	}
	void checkStatus (int portno1)
	{
		char p1[128];
	    char p2[128];
		int optval;
		char rbuf[20]="NULL";
	    char hb[20]="echo";
	    char rhb[20]="NULL";
	    int conn=0;
		int recvByte=0;
	    int connection; 
		struct sockaddr_in server1_addr;
	    struct sockaddr_in server2_addr;
	    struct sockaddr_in s2;
	    socklen_t socksize = sizeof(struct sockaddr_in);
	    int sfd,sfd1;
	    sfd = socket(AF_INET,SOCK_STREAM,0); 	/* creating socket for status*/
	    if (sfd <0)
			printf("Please enter the priority of this server: priority value for each server shoud be different(higher value high priority) \n" );
	    printf("Please enter the priority of this server: \n" );
	    scanf("%s", p1);
		memset(&server1_addr, 0, sizeof(server1_addr));
	    memset(&server2_addr, 0, sizeof(server2_addr));
		server1_addr.sin_family = AF_INET;
	    server1_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    server1_addr.sin_port = htons(portno1);
	    server2_addr.sin_family = AF_INET;
	    server2_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	    server2_addr.sin_port = htons(portno1);
	    if((connection=connect (sfd, (struct sockaddr*)&server2_addr , sizeof(struct sockaddr)))==0)
		{        
			printf("connect successful \n");
			send(sfd,status,sizeof(status),0);
			recvByte =  recv(sfd,rbuf,sizeof(rbuf),0);
			if (recvByte<0)
				printf("error in connect recv 12323 \n");	
			if (strcmp(rbuf,"init")==0 && strcmp(status,"init")==0)
			{
				send(sfd,p1,sizeof(p1),0);
				recv(sfd,p2,sizeof(p2),0);
				printf("priority of other connect server is %s \n",p2);
				if (strcmp(p1,p2)>0 )
				{	
					strcpy(status,"master");
					printf("statusof this server is: %s \n",status);
					communicate(status);
				}
				if (strcmp(p1,p2)<0 )
				{
					strcpy(status,"redunt");
					printf("this server is redundant server \n");
				}
				else if (strcmp(p1,p2)==0 )
					printf("priority level of master ans redundant cannot be  same\n");
			}
			else if (strcmp(rbuf,"master")==0 && strcmp(status,"init")==0)
			{
				strcpy(status,"redunt");
				printf("There is already server : %s  RUNNING..!!! \n",rbuf);
				printf("status of this server set to :%s  irrespective of priority\n",status);
			}
			while (strcmp(status,"redunt")==0) 			// loop it as long as this server status is redundant
			{	
				printf("initial status %s , sending my status :::: hb: %s\n", status,hb);
				send(sfd,hb,sizeof(hb),0);
				printf("first HB sent/recv is %s\n",rhb);
	            recvByte =  recv(sfd,rhb,sizeof(rhb),0);
				printf("first HB recv is %s\n",rhb);
				if (strcmp(rhb,hb)==0)
				{
					printf("sending/recieving echo from master \n");
					while (beatMissCount <5)
					{
						send(conn,hb,sizeof(hb),0);
						recvByte =  recv(sfd,rhb,sizeof(rhb),0);
		                if(recvByte <0)
						{
							beatMissCount++;
	                        printf("hearbeat send/miss failure: %d \n",beatMissCount);
	               		}
						else if (strcmp(rhb,"echo")==0) {
							beatMissCount =0;
							printf("echo response recived correctly\n");
							strcpy(rhb,"NULL");
						}
						else
						{	
							beatMissCount++;
							printf("hearbeat send/miss failure: %d \n",beatMissCount);
						}
						sleep (1);
					}
					strcpy(status,"master");
					printf("this is server status has changed to : %s \n",status);
					communicate(status);  //send this status master to parent process
				}
	            else	
	            {    
					printf("recived message from other server is %s \n", rhb);
	            }
					   sleep (1);
			} // end of redundant server loop
			close(sfd);
		}
		if (connection!=0 || strcmp(status,"master")==0)	//enter if connection is failed or status changed to master
		{
			printf("Waiting for other server... \n");
			sfd1 = socket(AF_INET,SOCK_STREAM,0);
			if (sfd<0)
				printf("error in creating sfd1 \n");
			optval = 1;
			setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	        bind(sfd1,(struct sockaddr*)&server1_addr,sizeof(server1_addr));   //x==0
			if (listen(sfd1,1)<0)
				printf("error in creating status listening \n");
			while(1)
			{
				conn = accept(sfd1, (struct sockaddr*)&s2 , &socksize);
	            if (conn <0)
					printf("error in accepting \n");
	            else
	            {
					printf("accept successful \n");
					recvByte =  recv(conn,rbuf,sizeof(rbuf),0);	
					printf("responding:: %s, status: %s\n",rbuf,status);
	                send(conn,status,sizeof(status),0);
	                if (strcmp(rbuf,"init")==0 && strcmp(status,"init")==0)
					{
						recv(conn,p2,sizeof(p2),0);
	                    send(conn,p1,sizeof(p1),0);
						printf("priority of other server is %s \n",p2);
						if (strcmp(p1,p2)>0 )
	                    {       
							strcpy(status,"master");
							printf("status of this server is: %s \n",status);
							communicate(status);
						}
	                    if (strcmp(p1,p2)<0 )
						{
							strcpy(status,"redunt");
							printf("Status of Server is: %s \n",status);
						}
	                    else if (strcmp(p1,p2)==0 )
							printf("priority level of both servers cannot be  same\n");
					}
					else if (strcmp(rbuf,"init")==0 && strcmp(status,"master")==0)
					{
						printf("I am already master and shall remain %s irrespective of priority \n",status);
					}
					else
					{
						printf("error listening_1010101:: rbuf: %s, status:: %s \n",rbuf,status);
					}
					while (strcmp(status,"master")==0) ///loop while status = master of the server
	                {
	                    recvByte =  recv(conn,rbuf,sizeof(rbuf),0);                      
				        if(recvByte <0)
	                    {
							beatMissCount++;
	                        printf("hearbeat send/miss failure: %d \n",beatMissCount);
	                    }
	                    else
						{
							if (strcmp(rbuf , "init")==0 && strcmp(status,"master")==0)
							{
								send(conn,status,sizeof(status),0);
							}
							else if (strcmp(rbuf,"echo")==0 && strcmp(status,"master")==0)
	                        {	
								send(conn,rbuf,sizeof(rbuf),0);
	                            while (beatMissCount <5)
								{
									recvByte =  recv(conn,rhb,sizeof(rhb),0);
									send(conn,rhb,sizeof(rhb),0);
	                                if(recvByte <0)
	                                {
										beatMissCount++;
	                                    printf("hearbeat send/miss failure: %d \n",beatMissCount);
	                                }
	                                else 
	                                {
										beatMissCount =0;
										strcpy(rhb,"NULL");
	                                }
								}
	                        }
							else
							{
								printf(" recived message from other server is %s \n", rbuf);
							}
	                    }
					}// end  of while status = master loop
				}
			} 		
			close(sfd1);
		}
	}
	
	int mainServer(char* cityArg)
	{
	    int optval;
	    threadData *tcpData;
	    pthread_t threads[100];
	    pthread_attr_t ta;
	    pthread_mutex_init(&mutexlock, NULL);
	    pthread_attr_init(&ta);
	    int listenfd = 0;
	    int connfd = 0;
	    int portno;
	    struct sockaddr_in serv_addr;
	    unsigned int clilen;
	    struct sockaddr_in cli_addr;
	    strcpy(currentCity,cityArg);
		if (strcmp(currentCity,"SJ")==0)
		{
		portno = 10000;
		}
		else if (strcmp(currentCity,"SF")==0)
		{
		portno = 10001;
		}
	    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (listenfd < 0)
	    {
	        printf("Error creating the listening socket.");
	        return -1;
	    }
		optval = 1;
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	    //zero out the structures and buffers
	    bzero(&serv_addr, sizeof(serv_addr));
		//set the server attributes
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    serv_addr.sin_port = htons(portno);
	   
	    //bind the listening socket
	    if(bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
	    {
	        printf("Could not bind the listening socket.\n");
	        return -1;
	    }
	        
	    //set the listening queue
	    if(listen(listenfd, QLEN) == -1)
	    {
	        printf("Failed to listen\n");
	        return -1;
	    }
	
	
	    //set up thread
	    (void) pthread_attr_init(&ta);
	    (void) pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
		printf("main server ready to accept request from client\n");	
		int connections = 0;
		clilen = sizeof(cli_addr);
	    while(1)
	    {
		
			if (connections == 99){
			connections = 0;
			}
		
	        connfd = accept(listenfd, (struct sockaddr*)&cli_addr,&clilen);
	        
	        if (connfd < 0)
	        {
	            if (errno == EINTR)
	                continue;
	            printf("accept: %s\n", strerror(errno));
	            return -1;
	        }
	    tcpData = malloc(sizeof(tcpData));
		tcpData->sock = connfd;
		tcpData->cli_addr = cli_addr;
		if(pthread_create(&threads[connections], &ta, tcpClientHandler,(void *)tcpData)){
			printf("Failed while creating TCP thread\n");
		}
	        connections = connections +1;
	    }
	    close(connfd);
		pthread_mutex_destroy(&mutexlock);
		pthread_attr_destroy(&ta);	
	}
	
	
	
	void *tcpClientHandler(void *tcp){
		struct sockaddr_in cli_addr;
		struct sockaddr_in serv_addr;
		struct hostent *server1 = gethostbyname("localhost");
		threadData *tcpDetail = tcp;	
		int newsockfd = tcpDetail->sock;
		int sent,received,sockfd,sent2,n2;	
		char *client_addr;		
		cli_addr = tcpDetail->cli_addr;
		char buffFile[15000] = {};
		char RecvBuffer[254] = {};
		char SendBuffer[254] = {};
		char passingBuffer[254] = {};
		char * typeOfClient;
		char * city;
		char * busNumber;
		char * busNumber2;
		char * busNumber3;
		char tempBusNum[30];
	    MYSQL *conn;
	    MYSQL *conn1;
	    MYSQL_RES *res;
	    MYSQL_ROW row;
	    char *server = "localhost";
	    char *user = "root";
	    char *password = "root"; 
	    char *database = "proj207";
	    conn = mysql_init(NULL);
		char str[80];
		char type2Rec[80]={};
		char sendBuf[254];
		char temp[254];
	
			
			client_addr = inet_ntoa(cli_addr.sin_addr);	
			printf("client address is %s \n",client_addr);
			printf("Client's port is %d\n",ntohs(cli_addr.sin_port));
			printf("============================================================\n");
			
		
		bzero(RecvBuffer,254);
		bzero(SendBuffer,254);	
		bzero(sendBuf,254);		
		received = recv(newsockfd,RecvBuffer,254,0);
		if (received < 0)
		{
			printf("Error while receiving\n");
			printf("The Server has been stopped\n");
			exit(1);
		}
		strcpy(passingBuffer,RecvBuffer);
			pthread_mutex_lock (&mutexlock);		
			typeOfClient = strtok(RecvBuffer,"~");
			pthread_mutex_unlock (&mutexlock);
			if (strcmp(typeOfClient,"type1") == 0) {
				//this is a type client
				pthread_mutex_lock (&mutexlock);		
				city =strtok(NULL,"~");
				busNumber= strtok(NULL,"~");	
				pthread_mutex_unlock (&mutexlock);
				printf("Type: %s \n",typeOfClient);
				printf("city: %s \n",city);
				printf("bus number: %s \n",busNumber);
		   if(strcmp(city,currentCity)==0)
		   {		
	   /* Connect to database */
	   if (!mysql_real_connect(conn, server,
	         user, password, database, 0, NULL, 0)) 
	         {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			 }
		strcpy(str,"select * from bus_details where bus_no =");
		strcat(str,busNumber);
		if (mysql_query(conn, str)) 
		{  
	      //finish_with_error(conn);
	      printf("failed to query\n");
		}
		res = mysql_use_result(conn);			
		while ((row = mysql_fetch_row(res)) != NULL){
	      printf("%s \n", row[1]);
	    	strcpy(sendBuf,row[1]);
			break;
	  }
	  mysql_free_result(res);  
	  mysql_close(conn);	
	}
	else {
		// server acting as client 
		printf("Going to get details from different server\n");	
		sockfd = socket(AF_INET,SOCK_STREAM,0);
			if (sockfd < 0)
			{
				printf("Error while creating the socket\n");
				exit(1);
			}
			server1 = gethostbyname("localhost");
			if (server1 == NULL)
			{
				printf("No such host exist\n");
				exit(1);
			}
			bzero((char *)&serv_addr,sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			bcopy((char *)server1-> h_addr,(char *)&serv_addr.sin_addr.s_addr,server1->h_length);
			if(strcmp(city,"SF")==0){
			serv_addr.sin_port = htons(10001);
		}
		else if(strcmp(city,"LA")==0){
			serv_addr.sin_port = htons(10002);
		}
		else if(strcmp(city,"SD")==0){
			serv_addr.sin_port = htons(10003);
		}
		else if(strcmp(city,"SJ")==0){
			serv_addr.sin_port = htons(10000);
		}
			if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))< 0)
			{
				printf("Error: Failed to connect\n");
				exit(1);
			}
			
			sent2 = send (sockfd,passingBuffer,254,0); 
			
			if(sent2<0)
			{
				printf("Error: Failed to send\n");
				exit(1);
			}
			
			
			bzero(sendBuf,100);
			n2 = recv(sockfd,sendBuf,254,0);
			if (n2< 0)
			{
			printf("Error: Failed while reading\n");
			exit(1);
			}
		
		close(sockfd);
		
		
			
		
		//server acting as client
	}
	if (strcmp(sendBuf,"")==0 ){
	strcpy(sendBuf,"Requested bus is not available");
	}
	
	if (strcmp(sendBuf,"1")==0 ){
	strcpy(sendBuf,"Bassett");	
	}
	        sent = send (newsockfd,sendBuf,254,0);
			if(sent<0)
			{
				printf("Error while sending\n");
				printf("The Server has been stopped\n");
				exit(1);
			}
	  printf("Done with processing client\n");
	  close(newsockfd);
	   	}
			else if(strcmp(typeOfClient,"type2") == 0){
				pthread_mutex_lock (&mutexlock); city =strtok(NULL,"~"); 
				busNumber= strtok(NULL,"~"); busNumber2= strtok(NULL,"~"); 
				busNumber3= strtok(NULL,"~"); pthread_mutex_unlock 
					(&mutexlock);
		if (!mysql_real_connect(conn, server,
	         user, password, database, 0, NULL, 0)) {
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
	   }
		strcpy(str,"select * from bus_details where bus_no in (");	
		strcat(str,busNumber);		
		strcat(str,",");
		strcat(str,busNumber2);
		strcat(str,",");
		strcat(str,busNumber3);
		strcat(str,")");
		if (mysql_query(conn, str)) {  
	      //finish_with_error(conn);
	      printf("failed to query\n");
		}
		int count=0;
		res = mysql_use_result(conn);			
		while ((row = mysql_fetch_row(res)) != NULL){
	    printf("%s \n", row[1]);
	    count++;
		printf("~\n");   
		   if(count == 1) strcpy(tempBusNum,busNumber);
		   else if(count == 2) strcpy(tempBusNum,busNumber2);
		   else if(count == 3) strcpy(tempBusNum,busNumber3);
		   else{
			strcpy(tempBusNum,"Error :");
	   }
	   strcat(type2Rec,"\n");
	   strcat(type2Rec,tempBusNum);
	   strcat(type2Rec," : ");
	   strcat(type2Rec,row[1]);		
	   printf("%s",type2Rec);
		}	
	  sent = send (newsockfd,type2Rec,254,0);
			if(sent<0)
			{
				printf("Error while sending\n");
				printf("The Server has been stoppedddd\n");
				exit(1);
			}
	  printf("Done with processing client\n");
	  close(newsockfd);
	  mysql_free_result(res);  
	  mysql_close(conn);	
	  	}
			else if(strcmp(typeOfClient,"set") == 0){
				printf("setting notification\n");
				pthread_mutex_lock (&mutexlock);		
				busNumber= strtok(NULL,"~");					
				pthread_mutex_unlock (&mutexlock);
	
		if (!mysql_real_connect(conn, server,
	         user, password, database, 0, NULL, 0)) {
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
			}
	strcpy(str,"select * from bus_details where bus_no =");
	strcat(str,busNumber);
		if (mysql_query(conn, str)) 
		{  
	      printf("failed to query\n");
		}
	  res = mysql_use_result(conn);			
	  while ((row = mysql_fetch_row(res)) != NULL)
	  {
	    printf("%s \n", row[1]);
		strcpy(sendBuf,row[1]);
		break;
	  }		
	  sent = send (newsockfd,sendBuf,254,0);
	  if(sent<0)
			{
				printf("Error while sending\n");
				printf("The Server has been stopped\n");
				exit(1);
			}
		printf("Done with processing client\n");
		strcpy(temp,sendBuf);
		int cond = 1;
		mysql_free_result(res);  
		while(cond==1){
		if (mysql_query(conn, str)) {  
	      printf("failed to query\n");
		}
		res = mysql_use_result(conn);			
		while ((row = mysql_fetch_row(res)) != NULL){
	    strcpy(sendBuf,row[1]);
		break;      
		}
		if(strcmp(sendBuf,temp)==0){
		}
		else{
		sent = send (newsockfd,sendBuf,254,0);
			if(sent<0)
			{
				printf("Error while sending\n");
				printf("The Server has been stopped_changed\n");
				break;
			}	
		strcpy(temp,sendBuf);
		}
	
	    mysql_free_result(res);  
		sleep(3);
		}
	  close(newsockfd);	
		}
			else if(strcmp(typeOfClient,"report") == 0){
				pthread_mutex_lock (&mutexlock);		
				busNumber= strtok(NULL,"~");					
				pthread_mutex_unlock (&mutexlock);
				
				if (!mysql_real_connect(conn, server,
	         user, password, database, 0, NULL, 0)) {
	      fprintf(stderr, "%s\n", mysql_error(conn));
	      exit(1);
	   }
					
				
		strcpy(str,"select * from bus_record  where bus_number =");
		strcat(str,busNumber);
	  if (mysql_query(conn, str)) {  
	      printf("failed to query\n");
	  }
	  res = mysql_use_result(conn);	
	
		while ((row = mysql_fetch_row(res)) != NULL)	
		{
		strcat(buffFile,row[0]);
		strcat(buffFile," was at location ");
		strcat(buffFile,row[1]);
		strcat(buffFile," at time ");		
		strcat(buffFile,row[2]);
		strcat(buffFile,"\n");      
		} 
	  mysql_free_result(res);  
	  mysql_close(conn);
	  if(send(newsockfd,buffFile,15000,0)<0)
		{  
		printf("Failed to proess\n");
		}  
	  }
			printf("End of prog\n");
	}
	
	
	
