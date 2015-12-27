CC= gcc

CFLAGS= -o 
CPthread= -ansi -Wall -pthread 

all: warsame_server warsame_client 

warsame_server: warsame_server.c
	$(CC) $(CFLAGS) warsame_server warsame_server.c $(CPthread)

warsame_client: warsame_client.c
	$(CC) $(CFLAGS) warsame_client warsame_client.c $(CPthread)

clean: 
	rm -rf *o run



