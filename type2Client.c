	//gcc -Wall -g -o type2Client type2Client.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	
	#include <gtk/gtk.h>
	#include <stdlib.h>
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
	
	void knowLocationType2(GtkButton *b1, gpointer  entry1)
	{
		char buff[256];
		int n=0;
		bzero(buff,strlen(buff));
		char msg[500];
		bzero(msg,strlen(msg));
		gchar busDetailString[500];
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
		serv_addr.sin_port=htons(SJPORTNO); 
	
		if (connect(SOCKFD,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		{
				printf("error in connecting with the server\n");
				exit(-1);
		}
	
			strcpy(buff,"type2~SJ~66~72~73~");
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
	
			strcpy(msg,"Current status of all Bus : ");
			
			strcat(msg,SENDBUFFER);
			puts(msg);
			strcpy(busDetailString,msg);
			gtk_label_set_text(entry1, busDetailString);
			bzero(msg,strlen(msg));
			bzero(busDetailString,strlen(busDetailString));
		}
		close(SOCKFD);
	}
	
	int main (int argc, char *argv[])
	{
	    GtkBuilder      *builder; 
	    GtkWidget       *window;
		GError *err = NULL;
	    gtk_init (&argc, &argv);
	
	    builder = gtk_builder_new ();
	    if(0 == gtk_builder_add_from_file (builder, "type2.glade", &err))
		{
			/* Print out the error. You can use GLib's message logging */
			fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
			/* Your error handling code goes here */
		}
	    window = GTK_WIDGET (gtk_builder_get_object (builder, "window2"));
	    gtk_builder_connect_signals (builder, NULL);
		
	    g_object_unref (G_OBJECT (builder));
	        
	    gtk_widget_show (window);  
	    gtk_main ();
	
	    return 0;
	}
	
