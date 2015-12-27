#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>

/*********************************************************************
 * Function Prototypes
 ********************************************************************/
void *forkConnection(void *);
void display_usage( char *, char * );
void error(char *);
int hostname_to_ip(char *, char *);

/*********************************************************************
 * Defines
 ********************************************************************/
#define OPTIONS "PORT"
#define PORT     60000
#define BUF_SIZE 1024
#define FOREVER 1

int sockfd, newsockfd, portno;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;
int *new_sock;
char ip[100];
int clients = 1;
