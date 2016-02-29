	//gcc -o stub $(mysql_config --cflags) stub.c $(mysql_config --libs)
	//create bus_record(bus_number int, bus_location varchar(255),bus_time varchar(255))
	
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
	#include <netdb.h>
	#include <mysql.h>
	#include <time.h>
	#define SA struct sockaddr
	
	int main(int argc,char *argv[])
	{
	
			char *stops72[] = {"\"Bassett\"","\"St.James\"","\"2nd and santa clara\"","\"San fernando\"","\"San Jose State University\"","\"San Carlos and 13th\"","\"San Antonio and 21st\"","\"McLaughlin and william\"","\"Sylvandale\"","\"Montery Hwy\""};
		   
			char *stops66[] = {"\"milpitas\"","\"Jacqulin\"","\"great mall\"","\"oakland\"","\"japantown\"","\"civic center\"","\"abel\"","\"SJ city hall\"","\"LRT stations\"","\"Russel strt\""};
		   
			char *stops73[] = {"\"San Fernando\"","\"SJSU\"","\"Keyas\"","\"Senter\"","\"santa clara lrt\"","\"martin luther king\"","\"st john\"","\"san carlos\"","\"seven trees\"","\"baroni\""};
		   
			char *stops323[] = {"\"cuppertino\"","\"santa clara\"","\"de anza college\"","\"San fernando\"","\"Almaden\"","\"Winchester\"","\"Wolfe\"","\"Julian\"","\"Harold\"","\"Miler\""};
		   
			char *stops22[] = {"\"sunnyvale\"","\"mountain view\"","\"page mill\"","\"castro\"","\"remington\"","\"scott\"","\"monroe\"","\"el camino\"","\"benton\"","\"railroad\""};
		   
			char *stops81[] = {"\"san carlos\"","\"newhall\"","\"west santa clara\"","\"park\"","\"stevens creek\"","\"wolfe\"","\"vallco hospital\"","\"1st streem\"","\"2nd street\"","\"9th street\""};
	       
	   int j = 0,n = 0;
	   int sockfd;
	   int len;
	   char compare[80];
	   MYSQL *conn;
	   MYSQL_RES *res;
	   MYSQL_ROW row;
	   char *server = "localhost";
	   char *user = "root";
	   char *password = "root";
	   char *database = "proj207";
	   conn = mysql_init(NULL);
	   if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) 
	   {
	      fprintf(stderr, "%s\n", mysql_error(conn));
	      exit(1);
	   }
	  
	char str[80];
	char buffer[10];
	char str1[80];
	char tempbuff[80];
	
	int value;
	int value1;
	time_t rawtime;
	
	while (1) 
	{ 
	       int n = 9;
	       int i = 0;
	for ( ; i < n ; i++)
	{
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops72[i]);
	strcat(str , str1);
	value1 = 72;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n", str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
	
	time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES(%d,%s,\"%s\")",value1,stops72[i],ctime(&rawtime));
	printf("%s\n" , compare);
	if (mysql_query(conn,compare))
	{      
	      printf("failed to query\n");
	}
	printf("checking here\n");
	strcpy(str , "");
	strcpy(str1 , "");
	strcpy(compare,"");
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops66[i]);
	strcat(str , str1);
	value1 = 66;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
	{      
	      printf("failed to query\n");
	}
	time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops66[i],ctime(&rawtime));
	printf("%s\n" , compare);
	if (mysql_query(conn,compare))
	{      
	      printf("failed to query\n");
	}
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops73[i]);
	strcat(str , str1);
	value1 = 73;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
	{      
	      printf("failed to query\n");
	}
	time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops66[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
	{      
	      printf("failed to query\n");
	}
		  strcpy(str , "");
		  strcpy(str1 , "");
	      strcpy(compare,"");
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops323[i]);
	strcat(str , str1);
	value1 = 323;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
	time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops323[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
		  {      
	      printf("failed to query\n");
		  }
		  strcpy(str , "");
		  strcpy(str1 , "");
	          strcpy(compare,"");
		 
		  
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops22[i]);
	strcat(str , str1);
	value1 = 22;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
		  time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops22[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
		  {      
	      printf("failed to query\n");
		  }
		  strcpy(str , "");
		  strcpy(str1 , "");
	          strcpy(compare,"");
		 
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops81[i]);
	strcat(str , str1);
	value1 = 81;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
		  time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops81[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
		  {      
	      printf("failed to query\n");
		  }
		  strcpy(str , "");
		  strcpy(str1 , "");
	          strcpy(compare,"");	
		  
	
	sleep(5);
	}
	
	////////////////////////////////////////////////////////////////////////
	for ( i = n ; i >= 0 ; i--)
	{
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops72[i]);
	strcat(str , str1);
	value1 = 72;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
	time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops72[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
		  {      
	      printf("failed to query\n");
		  }
		  strcpy(str , "");
		  strcpy(str1 , "");
	          strcpy(compare,"");
	
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops66[i]);
	strcat(str , str1);
	value1 = 66;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
	time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops66[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
		  {      
	      printf("failed to query\n");
		  }
		  strcpy(str , "");
		  strcpy(str1 , "");
	          strcpy(compare,"");
		  
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops73[i]);
	strcat(str , str1);
	value1 = 73;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
	time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops73[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
		  {      
	      printf("failed to query\n");
		  }
		  strcpy(str , "");
		  strcpy(str1 , "");
	          strcpy(compare,"");
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops323[i]);
	strcat(str , str1);
	value1 = 323;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
	time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops323[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
		  {      
	      printf("failed to query\n");
		  }
		  strcpy(str , "");
		  strcpy(str1 , "");
	          strcpy(compare,"");
		 
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops22[i]);
	strcat(str , str1);
	value1 = 22;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
		  strcpy(str , "");
		  strcpy(str1 , "");
		 
		  
	strcpy(str , "update bus_details set bus_location = ");
	strcpy(str1 , " where bus_no = ");
	strcat(str , stops81[i]);
	strcat(str , str1);
	value1 = 81;
	sprintf(buffer, "%d", value1);
	strcat(str,buffer);
	printf("%s\n" , str);
	if (mysql_query(conn,str))
		  {      
	      printf("failed to query\n");
		  }
		 time (&rawtime);
	//sprintf(compare,"INSERT INTO bus_record VALUES (%d,%s,\"%s\")",value1,stops81[i],ctime(&rawtime));
	printf("%s\n" , str);
	if (mysql_query(conn,compare))
	{      
	printf("failed to query\n");
	}
	strcpy(str , "");
	strcpy(str1 , "");
	strcpy(compare,"");
	sleep(5);
	}    
	}
	   return 0 ; 
	   
	
	}
	
