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

#include "include/client_tools.h"
#include "include/common_tools.h"

#define BUFF_LEN_MAX 1000
#define BACKLOG 21
#define PSEUDO_LEN_MAX 100
// ----------- MAIN ------------- //

int main(int argc,char** argv){

  char* msg_cli = malloc(BUFF_LEN_MAX*sizeof(char));
  char* msg = malloc(BUFF_LEN_MAX*sizeof(char));
  char* death_msg = "_kill_";

  memset(msg_cli,0,sizeof(msg_cli));

  char * ip = argv[1];
  int port = atoi(argv[2]);

  if (argc != 3)
  {
    fprintf(stderr,"usage: RE216_CLIENT hostname port\n");
    return 1;
  }

  //get address info from the server
  struct sockaddr_in serv_addr;
  //serv_addr = malloc(sizeof(struct sockaddr_in));

  get_addr_info(argv[2], &serv_addr, ip);


  //get the socket
  int s = do_socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);

  //connect to remote socket
  char*msg_connection = malloc(100*sizeof(char));

  int connect = do_connect(s, serv_addr);
  do_recv(s,msg_connection);


  if(strncmp(msg_connection,death_msg,7)==0){
    printf("Too many connection. Server close connection \n");
    close(s);
  }
  else if(strncmp(msg_connection,"server_client",14)==0){
    do_send(s,ip,strlen(ip));

    printf("Connection with server ok \n");
    printf("[SERVER] : Please login with /nick <your pseudo> \n");
    for(;;){
      handle_client_message(s,msg_cli,port);
      do_send(s,msg_cli,strlen(msg_cli));
      do_recv(s,msg);
      printf("[ SERVER ] :%s\n", msg);

      if(strncmp(msg_cli,"/quit",5)==0){
        break;
      }

      else if(strncmp(msg_cli,"/who",4)==0  && strncmp(msg_cli, "/whois",6)!=0){
        char msg_who[PSEUDO_LEN_MAX*20];
        do_recv(s,msg_who);
        printf("[SERVER : ] Online users are : %s\n",msg_who);
        fflush(stdout);
        memset(msg_who,'\0',BUFF_LEN_MAX*sizeof(char));
      }

      else if(strncmp(msg_cli, "/whois",6)==0 && strncmp(msg_cli, "/who",4)==0){
        char msg_whois[5000];
        do_recv(s,msg_whois);
        printf("[SERVER : ] %s\n",msg_whois);
        fflush(stdout);
        memset(msg_whois,'\0',2000*sizeof(char));
      }

      //Memory libeation
      memset(msg_cli,'\0',BUFF_LEN_MAX*sizeof(char));
      memset(msg,'\0',BUFF_LEN_MAX*sizeof(char));
      
    }
    //close Connection
    printf("Close connection\n");
    close(s);
  }

  return 0;


}
