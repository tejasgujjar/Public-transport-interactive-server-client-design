	#include <gtk/gtk.h>
	#include <stdlib.h>
	#include <mysql.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <errno.h>
	#include <arpa/inet.h>
	#include <pthread.h> 
	
	#define SFPORTNO 10001
	#define SJPORTNO  10000
	
	/* Global variables */
	int SanFransico[]= {66 ,67 ,68};
	int SanJose[]= {72 ,75 ,76};
	const gchar *USERNAME;
	const gchar *PASSWORD;
	const gchar *ENTERCITY;
	const gchar *ENTERBUSNUMBER;
	const gchar *ENTERBUSNUMBERNOTIFY;
	int connected =0;
	char USERCITY[5];
	
	
	void on_window1_destroy (GObject *object, gpointer user_data)
	{
		gtk_main_quit();
	}
	
	
	
	gboolean entercity(GtkEntry *e1, gpointer  user)
	{
		ENTERCITY = gtk_entry_get_text ((e1));
		return FALSE;
	}
	
	gboolean enterbusnumber(GtkEntry *e1, gpointer  user)
	{
		ENTERBUSNUMBER = gtk_entry_get_text ((e1));
		return FALSE;
	}
	gboolean enteredBusNotify(GtkEntry *e1, gpointer  user)
	{
		ENTERBUSNUMBERNOTIFY = gtk_entry_get_text ((e1));
		return FALSE;
	}
	
	gboolean enteredUsername(GtkEntry *e1, gpointer  user)
	{
		USERNAME = gtk_entry_get_text ((e1));
		return FALSE;
	}
	gboolean enteredPassword(GtkEntry *e2, gpointer  user)
	{
		PASSWORD = gtk_entry_get_text ((e2));
		return FALSE;
	}
	
	void fetched(GtkButton *b1, gpointer  entry1)
	{
		char buff[256];
		int n=0;
		bzero(buff,strlen(buff));
		char msg[500];
		bzero(msg,strlen(msg));
		gchar busDetailString[500];
		printf("Entered City Name: %s\n",ENTERCITY);
		printf("Entered Bus Number: %s\n",ENTERBUSNUMBER);
		int portno;
		char SENDBUFFER[256];
		int SOCKFD = 0;
		struct sockaddr_in serv_addr;	
		struct hostent *server1;
		if( (SOCKFD = socket(AF_INET,SOCK_STREAM,0) ) <  0 )
		{
			printf("client socket failied");
			exit(-1);
		}
		bzero(SENDBUFFER,strlen(SENDBUFFER));
		bzero((char *) &serv_addr, sizeof(serv_addr));
		server1 = gethostbyname("localhost");
		serv_addr.sin_family=AF_INET;
		bcopy((char *)server1->h_addr,(char *)&serv_addr.sin_addr.s_addr,server1->h_length);
	         if ( strcmp(USERCITY,"SF") == 0)
	         {
				 
				serv_addr.sin_port=htons(SFPORTNO); 
			 } else if (strcmp(USERCITY,"SJ") == 0)
			{
				
			 serv_addr.sin_port=htons(SJPORTNO); 
			}
			else{
				printf("Invalid command from user\n");
				exit(-1);
				}
		
		if (connect(SOCKFD,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		{
				printf("error in connecting with the server\n");
				exit(-1);
		}
		else {
		
			connected = 1;
		}
	
			strcpy(buff,"type1~");
			strcat(buff,ENTERCITY);
			strcat(buff,"~");
			strcat(buff,ENTERBUSNUMBER);
			strcat(buff,"~");
			strncpy(SENDBUFFER,buff,strlen(buff));
	
	
		n = write(SOCKFD,SENDBUFFER,strlen(SENDBUFFER));
		if(n<0)
		{
			printf("error in sending\n");
			exit(-1);
		}
		bzero(SENDBUFFER,strlen(SENDBUFFER));
		n = read(SOCKFD,SENDBUFFER,sizeof(SENDBUFFER) -1 );
		if(n<0)
		{
			perror("error in reading \n");
		}
		else{
			strcpy(msg,"Location : ");
			strcat(msg,SENDBUFFER);
			puts(msg);
			strcpy(busDetailString,msg);
			gtk_label_set_text(entry1, busDetailString);
			bzero(msg,strlen(msg));
			bzero(busDetailString,strlen(busDetailString));
		}
		close(SOCKFD);
	}
	
	void setNotification(GtkButton *b1, gpointer  entry2)
	{
		if(fork() == 0){
		char buff[256];
		int n=0;
		bzero(buff,strlen(buff));
		printf("Entered Bus Number for Notification is: %s\n",ENTERBUSNUMBERNOTIFY);
		int portno;
		char SENDBUFFER[256];
		int SOCKFD = 0;
		struct sockaddr_in serv_addr;
		struct hostent *server1;
		if( (SOCKFD = socket(AF_INET,SOCK_STREAM,0) ) <  0 )
		{
			printf("client socket failied");
			exit(-1);
		}
		bzero(SENDBUFFER,strlen(SENDBUFFER));
		bzero((char *) &serv_addr, sizeof(serv_addr));
		server1 = gethostbyname("localhost");
		serv_addr.sin_family=AF_INET;
		bcopy((char *)server1->h_addr,(char *)&serv_addr.sin_addr.s_addr,server1->h_length);
	         if ( strcmp(USERCITY,"SF") == 0)
	         {
				serv_addr.sin_port=htons(SFPORTNO); 
			 } else if (strcmp(USERCITY,"SJ") == 0)
			{
			 serv_addr.sin_port=htons(SJPORTNO); 
			}
			else{
				printf("Invalid command from user\n");
				exit(-1);
			}
		if (connect(SOCKFD,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		{
				printf("error in connecting with the server\n");
				exit(-1);
		}
		else {
			connected = 1;
		}
			strcpy(buff,"set~");
			strcat(buff,ENTERBUSNUMBERNOTIFY);
			strcat(buff,"~");
			strncpy(SENDBUFFER,buff,strlen(buff));
		n = write(SOCKFD,SENDBUFFER,strlen(SENDBUFFER));
		if(n<0)
		{
			printf("error in sending\n");
			exit(-1);
		}
		bzero(SENDBUFFER,strlen(SENDBUFFER));
		n = read(SOCKFD,SENDBUFFER,sizeof(SENDBUFFER) -1 );
			if(n<=0)
			{
				perror("error in reading \n");
				close(SOCKFD);
				exit(-1);
			}
			else{
			printf("\n----------------Notification  Alert-------------------\n");
			printf("Current Location of bus number %s is : %s\n",ENTERBUSNUMBERNOTIFY,SENDBUFFER);
			printf("------------------------------------------------------\n");
			}
		while(1)
		{
			n = read(SOCKFD,SENDBUFFER,sizeof(SENDBUFFER) -1 );
			if(n<=0)
			{
				perror("error in reading \n");
				close(SOCKFD);
				exit(-1);
			}
			else{
			printf("\n----------------Notification  Alert-------------------\n");
			printf("Current Location of bus number %s is : %s\n",ENTERBUSNUMBERNOTIFY,SENDBUFFER);
			printf("------------------------------------------------------\n");
			}
		}
	
			close(SOCKFD);
		}
		else {
			printf("Returning from parent\n");
		}
	}
	
	
	void submit_clicked(GtkButton *b1, gpointer  entry1)
	{
		int databaseResponse =0;
		MYSQL *conn;
	   char *server = "localhost";
	   char *user = "root";
	   char *password = "root";
	   char *database = "tejas";
	    char str[80];
		printf("Entered Username: %s\n",USERNAME);
		printf("Entered Password: %s\n",PASSWORD);
	   conn = mysql_init(NULL);   
	   if (!mysql_real_connect(conn, server,
	         user, password, database, 0, NULL, 0)) {
	      fprintf(stderr, "%s\n", mysql_error(conn));
	      exit(1);
	   }
	   sprintf(str , "Select  count(*) from user_details where username='%s' and password='%s'" , USERNAME,PASSWORD);
	
	    if (mysql_query(conn, str))
			{  
				//finish_with_error(conn);
				printf("failed to query");
			}
			
			MYSQL_RES *res = mysql_store_result(conn);
			
			 MYSQL_ROW row;
	  
	 while ((row = mysql_fetch_row(res))) 
	  { 
	      printf("%s ", row[0]); 
	          printf("\n"); 
	          if(strcmp(row[0],"0") == 0)
	          {
				databaseResponse = 0;
				}
				else if(strcmp(row[0],"0") != 0)
				{
					databaseResponse = 1;
					}
					else
					{
						printf("Invalid Username or Password!!! \n");
						exit(-1);	
						}
	  }
		 if(databaseResponse == 0)
		 {
			 printf("Invalid Username or Password!!! \n");
			 exit(-1);
		 }
		 else if(databaseResponse == 1)
		 {
			 	 /*  open new window  */
	    GtkBuilder      *builderNew; 
	    GtkWidget       *windowNew;
		GError *err = NULL;
	    gtk_init (NULL, NULL);
		builderNew = gtk_builder_new ();
	    if(0 == gtk_builder_add_from_file (builderNew, "busDetails.glade", &err))
		{
			/* Print out the error. You can use GLib's message logging */
			fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
			/* Your error handling code goes here */
		}
	    windowNew = GTK_WIDGET (gtk_builder_get_object (builderNew, "window2"));
	    gtk_builder_connect_signals (builderNew, NULL);
	    g_object_unref (G_OBJECT (builderNew));
	    gtk_widget_show (windowNew);                
	    gtk_main ();
		 }
		 else
		 {
			 printf("Invalid Username or Password!!! \n");
			 exit(-1);
		 }
		mysql_free_result(res);
		mysql_close(conn);
	}
	
	int main (int argc, char *argv[])
	{
	    GtkBuilder      *builder; 
	    GtkWidget       *window;
		GError *err = NULL;
	    gtk_init (&argc, &argv);
		strcpy(USERCITY,argv[1]);
	    builder = gtk_builder_new ();
	    if(0 == gtk_builder_add_from_file (builder, "user2.glade", &err))
		{
			/* Print out the error. You can use GLib's message logging */
			fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
			/* Your error handling code goes here */
		}
	    window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
	    gtk_builder_connect_signals (builder, NULL);
		
	    g_object_unref (G_OBJECT (builder));
	        
	    gtk_widget_show (window);  
	    gtk_main ();
	
	    return 0;
	}
	
