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



#define BUFF_LEN_MAX 1000

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

char* handle_client_message(int sockfd, char*buffer){
  printf("Write your message : \n");
  fflush(stdout);
  fgets(buffer, BUFF_LEN_MAX, stdin); //read the message in stdin
  return buffer;
}

void do_send(int sockfd, char* msg, int len) {
  int retour;
  size_t taille = (size_t)len;
  void * cast_msg = (void*)msg;
  retour = send(sockfd, cast_msg, taille, 0);
  if (retour == -1)
  printf("error : send");
}

//Read message
void do_recv(int sockfd, void*buff){
  int msg_recv;
  msg_recv = recv(sockfd, buff, BUFF_LEN_MAX, 0);
  if (msg_recv == -1)
  printf("reception error");
}


int main(int argc,char** argv){

  char* msg_cli = malloc(BUFF_LEN_MAX*sizeof(char));
  void* msg= malloc(BUFF_LEN_MAX*sizeof(char));


  memset(msg_cli,0,sizeof(msg_cli));

  char * ip = argv[1];

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
  int connect = do_connect(s, serv_addr);


  struct pollfd fds[2];

  memset(fds,0,sizeof(fds));

  //pour les messages envoyés du serveur
  fds[0].fd = s;
  fds[0].events = POLLIN;
  fds[0].revents = 0;
  //pour les messages envoyés depuis le client
  fds[1].fd = 0;
  fds[1].events = POLLIN;



  for(;;){
    int rc = poll(fds,2,-1);

    if(fds[0].revents == POLLIN){
      do_recv(s,msg);
      char *msg_ser =(char*)msg;
      if (strncmp(msg_ser, "close", 5)==0){
        printf("error in connection, to many clients\n");
      }
      break;
    }

    else {
      printf("Connection with server ok \n");
      handle_client_message(s,msg_cli);
      fflush(stdout);
      do_send(s,msg_cli,strlen(msg_cli));
      do_recv(s,msg);
      char*msg_ser =(char*)msg;
      printf("Server says :%s\n", msg_ser);
      if(strncmp(msg_cli,"/quit",5)==0){
        break;
      }
    }
      //Memory libeation
      memset(msg_cli,0,sizeof(msg_cli));
    }
  //close Connection
  close(s);



  return 0;


}
