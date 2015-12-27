#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>

/*********************************************************************
 * Function Prototypes
 ********************************************************************/
void display_usage( char *, char * );
void error(char *);

/*********************************************************************
 * Defines
 ********************************************************************/
#define OPTIONS "HOSTNAME PORT"
#define BUF_SIZE 1024
#define FOREVER 1
#define h_addr h_addr_list[0] /* for backward compatibility */

int sockfd, portno, n, fd, m;
struct sockaddr_in serv_addr;
char buffer[BUF_SIZE];
char bufferTwo[BUF_SIZE];
struct hostent *server, h_addr;
