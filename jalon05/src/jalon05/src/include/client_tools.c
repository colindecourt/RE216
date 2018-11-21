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

struct sockaddr_in init_client_addr(int port)
{
    struct sockaddr_in client_addr;
    memset(&client_addr, '\0', sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    inet_aton("127.0.0.1", &client_addr.sin_addr);
    return client_addr;
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


