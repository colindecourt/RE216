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

#include "common_tools.h"


#define BUFF_LEN_MAX 1000
#define PSEUDO_LEN_MAX 100
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20

// ------------------------------------------------------------- //

int do_socket(int domain, int type, int protocol) {
  int sockfd;
  int yes = 1;
  //create the socket
  sockfd = socket( domain, type, protocol );
  //check for socket validity
  if (sockfd == -1){
    fprintf(stderr,"Error : Unvalid socket\n");
  }
  // set socket option, to prevent "already in use" issue when rebooting the server right on
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
  fprintf(stderr,"ERROR setting socket options\n");

  return sockfd;
}

// ------- Read message -------- //

int do_recv(int sockfd, char *buff){
  int msg_recv;
  void * buffer = (void*)buff;
  msg_recv = recv(sockfd, buffer, BUFF_LEN_MAX, 0);
  if (msg_recv == -1)
  printf("reception error");
  return msg_recv;
}

// ------ Send message ------- //

int do_send(int sockfd, char* msg, int len) {
  int retour;
  void * cast_msg = (void*)msg;
  retour = send(sockfd, cast_msg, strlen(msg), 0);
  if (retour == -1)
  printf("error : send");
  return retour;
}
