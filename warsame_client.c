/*****************************************************************************
 FILE      :  warsame_client.c
 PROGRAM   :  Network Applications
 AUTHOR    :  Warsame Egal
 DUE DATE  :  December 10, 2015
 COURSE    :  CST8234, Carolina Ayala
 PURPOSE   :  To create a Server – Client application over the network
*****************************************************************************/

/***********************************************
 Include private library headers.
***********************************************/
#include "client.h"

/*****************************************************************************
  FUNCTION :  main
  PURPOSE  :  Creates the client and connects to the server
 ******************************************************************************/
int main(int argc, char *argv[]) {

  if (argc < 3) {
    display_usage( argv[0], OPTIONS );
    exit( EXIT_FAILURE );
  }
	
  portno = atoi(argv[2]);

  /*Display menu information*/
  printf("\n");
  printf("CLIENT: Welcome to the Client/Server chat:\n");
  printf("CLIENT: Simply enter a text below to comminicate with the server:\n");
  printf("CLIENT: Open a another terminal and connect as a second client if you wish\n");
  printf("CLIENT: Enter Q or q to terminate server:\n");
  printf("\n");

 /*
  * Step 1:  Create a socket 
  */
  if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
    error("CLIENT: ERROR opening socket");

 /* Given the hostname, returns the IP address*/
  if ( (server = gethostbyname(argv[1]) ) == NULL ) {
    fprintf(stderr,"CLIENT: ERROR, no such host\n");
    exit(0);
  }

  printf("CLIENT: Socket Created:\n");

 /*
  * Init server address
  */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
  (char *)&serv_addr.sin_addr.s_addr,server->h_length);
  serv_addr.sin_port = htons(portno);

 /*
  * Step 2:  Connect
  */
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");
       
  printf("CLIENT: Connected\n");
  printf("\n");

 /*
  * Chat, send and recieve information to and from the server
  */
  while (FOREVER) {
    printf("Client: Enter a message: ");
    fgets(buffer, 255, stdin);
    printf("CLIENT: Message being sent: %s\n", buffer);
    send(sockfd, buffer, strlen(buffer), 0);
    if (*buffer == 'q' || *buffer == 'Q') {
      printf("CLIENT: Goodbye\n");
      exit(EXIT_SUCCESS);
    }
    recv(sockfd, buffer, strlen(buffer), 0);
    printf("CLIENT: Server reply: ");
    puts(buffer); 
  }

  close(sockfd);

  return 0;
}

/*****************************************************************************
  FUNCTION :  display_usage
  PURPOSE  :  Displays usage to the user
 ******************************************************************************/
void display_usage( char * prog, char *opts ) {
  fprintf(stderr, "usage: %s %s\n", prog, opts );
  return;
}

/*****************************************************************************
  FUNCTION :  error
  PURPOSE  :  Error and Exit
 ******************************************************************************/
void error(char *msg) {
  perror(msg);
  exit( EXIT_FAILURE );
}

