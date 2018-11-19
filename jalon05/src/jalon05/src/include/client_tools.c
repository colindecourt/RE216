#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client_tools.h"



// ------------ FONCTIONS ------------ //

#define BUFF_LEN_MAX 1000
#define PSEUDO_LEN_MAX 100
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20

struct sockaddr_in get_addr_info(const char* port, struct sockaddr_in* serv_addr1, char* ip) {
  int portno;

  //clean the serv_add structure
  memset( serv_addr1, '\0', sizeof(struct sockaddr_in));

  //cast the port from a string to an int
  portno   = atoi(port);

  //internet family protocol
  serv_addr1->sin_family = AF_INET;

  //we bind to any ip form the host
  serv_addr1->sin_addr.s_addr = inet_addr(ip);

  //we bind on the tcp port specified
  serv_addr1->sin_port = htons(portno);
}



// ------------------------------------------------------ //

int do_connect(int sockfd, const struct sockaddr_in addr) {
  int res = connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
  if (res != 0) {
    //fprintf(stderr,"Error : Unable to connect\n");
    perror("error connection : ");
    exit(EXIT_FAILURE);
  }
  //printf("Connection with server\n");
  return res;
}

// -------------------------------------------- //

char* handle_client_message(int sockfd, char*buffer, int port){
  memset(buffer,'\0',BUFF_LEN_MAX);
  printf("[ CLIENT port n° : %i ] : ", port);
  fflush(stdout);
  fgets(buffer, BUFF_LEN_MAX, stdin); //read the message in stdin
  //faire fonction qui détecte le \n
  return buffer;
}

// -------------------------------------------------------- //

int to_display(int display, char * channel_name, char*pseudo)
{
  if(display == 0)
  {
    printf(">> %s :",pseudo);
    fflush(stdout);
  }

  else if (display == 1)
  {
    printf("[ %s ] ",channel_name);
    fflush(stdout);
  }
}


