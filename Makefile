#*****************************************************************
#
# CMPE 207 (Network Programming and Applications) Project Makefile
# Title: Public-Transport Interactive Client-Server Design
#
#*****************************************************************

all: type1Client type2Client type3Client stub servers 

type1Client: type1.c
	gcc type1.c -o testGlade  `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs` -export-dynamic -rdynamic

type2Client:	type2Client.c
	gcc -Wall -g -o type2Client type2Client.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

type3Client:	type3.c
	gcc -Wall -g -o type3 type3.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -rdynamic

stub:	stub.c
	gcc -o stub $(mysql_config --cflags) stub.c $(mysql_config --libs) `mysql_config --cflags --libs`

servers: servers.c
	gcc servers.c -o servers  `mysql_config --cflags --libs` -pthread

clean:
	rm servers stub type3 type2Client testGlade

