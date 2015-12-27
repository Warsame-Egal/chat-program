/*****************************************************************************
 FILE      :  warsame_server.c
 PROGRAM   :  Network Applications
 AUTHOR    :  Warsame Egal
 DUE DATE  :  December 10, 2015
 COURSE    :  CST8234, Carolina Ayala
 PURPOSE   :  To create a Server – Client application over the network
*****************************************************************************/

/***********************************************
 Include private library headers.
***********************************************/
#include "server.h"

/*****************************************************************************
  FUNCTION :  main
  PURPOSE  :  Creates the server connection with socket
 ******************************************************************************/
int main(int argc, char *argv[]) {
  char *port = argv[1];

  /*Display the server menu*/
  printf("\n");
  printf("\n");
  printf("\t**************************************************************\n");
  printf("\n");
  printf("\n");
  printf("\tWelcome to the Client/Server chat program by Warsame Egal\n");
  printf("\tYou can either use a single or multiple connections when \n");
  printf("\tconnecting to the server\n");
  printf("\n");
  printf("\tThe Servers purpose is to recieve and display data from \n");
  printf("\tthe client which then forwards the message back to the client.\n");
  printf("\n");
  printf("\n");
  printf("\t**************************************************************\n");
  printf("\n");

  /*Display usage if aruements is less than 2*/
  if (argc < 2) {
    display_usage( argv[0], OPTIONS );
    exit( EXIT_FAILURE );
  } 
	
 /*
  * Step 1:  Create a socket 
  * socket( ) requieres 3 arguments:
  * domain:  AF_UNIX --> Unix internal protocol -- same filesystem
  * AF_INET --> Internel, any two host 
  * type_socket:
  * SOCK_STREAM --> sequence two way connection based on bytes
  * SOCK_DGRAM  --> datagrams, info read in chuncks
  * protocol: if 0 --> OS choose
  */
  if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) 
    error( "SERVER: Open Socket" );

 /*
  * Init serv_add to zeros
  */
  bzero( (struct sockaddr_in *) &serv_addr, sizeof(serv_addr) );

  portno = atoi(argv[1]);

 /*
  * Init serv_addr
  * struct sockaddr_in {
  * short   sin_family;
  * u_short sin_port;
  * struct  in_addr s_addr;
  * char    sin_zero[8];
  * s_add --> IP address of the host  INADDR_ANY is a constant with the IP 
  */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
     
 /* 
  * Bidning socket
  * bind( ) binds a socket to an address
  */
  if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  
    error("SERVER: ERROR on binding");
   
  printf("SERVER: Binding successuful\n");

  /*Listen to socket*/
  listen( sockfd, 3);

  /*Accept the connection
  * Display hostname and ip address
  */
  printf("SERVER: Waiting for incoming connection\n");
  hostname_to_ip(port , ip);
  printf("SERVER: %s resolved to %s\n" , port , ip);
  clilen = sizeof(cli_addr);

  /*Accept the incomming connection from the client*/
  while(newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) {
    if (newsockfd < 0) {
      error("SERVER: Accept-failed\n");
      return 1;
    }

    /*Conneciton accepted, create a thread*/
    printf("SERVER: Client number %d\n", clients);
    clients++; /*number of clients*/

    /*create pthread for connection*/
    pthread_t sniff;
    new_sock = malloc(1);
    *new_sock = newsockfd;
    if (pthread_create(&sniff,NULL,forkConnection,new_sock) < 0 ) {
      error("SERVER: Could not create thread\n");
      return 1;
    }
  }
  /*error checking*/
  if(new_sock == NULL) { 
    error("SERVER: Falied to accept\n");
  }

  return 0; 
}

/*****************************************************************************
  FUNCTION :  forkConnection
  PURPOSE  :  Creates the server connection with socket
              forkConnection handles every client
              forks new process for every client
              reads and writes bytes back and fourth
              while displaying the information
              displays the date and time for every conversation
 ******************************************************************************/
void *forkConnection(void *socket_fork) {
  int sock = *(int *) socket_fork;
  char buffer[BUF_SIZE];
  int n;
  pid_t pid;
  char **args = NULL;
  time_t current_time;
  char* c_time_string;

  bzero(buffer,256);

  /*Reviecing message from client*/
  while(( n = recv(sock, buffer,BUF_SIZE, 0)) > 0 ) {

    /* Obtain current time. */
    current_time = time(NULL);

    /*obtain the current time*/
    if (current_time == ((time_t)-1)) {
      (void) fprintf(stderr, "Failure to obtain the current time.\n");
      exit(EXIT_FAILURE);
    }

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    if (c_time_string == NULL) {
      (void) fprintf(stderr, "Failure to convert the current time.\n");
      exit(EXIT_FAILURE);
    }

    /* if recv returns a 0 error*/
    if (n == 0) {
      error("SERVER: Cannot recieve data from Client\n");
      exit(EXIT_FAILURE);
    }

    /* if recv error check*/
    else if (n == -1) {
      error("SERVER: Error in recv\n");
      exit(EXIT_FAILURE);
    }

    /*If client inputs a q, server will terminate*/
    if (*buffer == 'q' || *buffer == 'Q') {
      printf("\n");
      printf("\n");
      printf("SERVER: Goodbye\n");
      close(sock);
      exit(EXIT_FAILURE);
    }
    
    /*Fork a new process*/
    pid = fork();
      
    /*error*/
    if (pid == -1 ) { 
      error("Fork");
      exit(EXIT_FAILURE);
    }
      
    /*child*/
    if (pid == 0) { 
      printf("\n");
      printf("SERVER: Retriving Message\n");
      printf("\n");
      printf("\n");
      printf("\t**************************************************************\n");
      printf("\t%s\n", c_time_string);
      printf("\tSERVER:\t\t\t%s", buffer);
      printf("\t**************************************************************\n");
      write(sock, buffer, strlen(buffer)); /*Write to the client*/
      fprintf(stderr, "Exec: %s %s\n",args[0], strerror(errno));    
      exit(EXIT_FAILURE);            
    }
    
    /*parent*/
    else { 
      bzero(buffer, 256); /*clrea the buffer*/
      wait(NULL); /*wait for child to finish then proceed*/    
    } 
  }
  close(sock);
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

/*****************************************************************************
  FUNCTION :  hostname_to_ip
  PURPOSE  :  get the hostname and ip address
 ******************************************************************************/
int hostname_to_ip(char * hostname , char* ip) {
  struct hostent *he;
  struct in_addr **addr_list;
  int i;
         
  if ( (he = gethostbyname( hostname ) ) == NULL) {
    error("gethostbyname"); /*get the host info*/
    return 1;
  }

  addr_list = (struct in_addr **) he->h_addr_list;
     
  for(i = 0; addr_list[i] != NULL; i++) {
    strcpy(ip , inet_ntoa(*addr_list[i]) ); /*Return the first one;*/
    return 0;
  }
     
  return 1;
}
