#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <error.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>


struct sockaddr_in get_addr_info(const char* port, struct sockaddr_in* serv_addr1, char* ip) {
  int portno;

  //clean the serv_add structure
  memset( serv_addr1, '\0', sizeof(struct sockaddr_in));

  //cast the port from a string to an int
  portno   = atoi(port);

  //internet family protocol
  serv_addr1->sin_family = AF_INET;

  //we bind to any ip form the host
  //inet_aton("127.0.0.1", & serv_addr.sin_addr);
  serv_addr1->sin_addr.s_addr = inet_addr(ip);
  //serv_addr->sin_addr->s_addr = htonl("127.0.0.1");

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
  return res;
}


ssize_t readline(int sockfd, void*str, size_t maxlen){
  ssize_t msg = read(sockfd, str, maxlen);
  if(msg == -1){
    printf("Unable to read message");
  }
  else
    return msg;
}

void handle_client_message(int sockfd, char*buffer){
  printf("Write your message");
  fgets(buffer, sizeof(buffer), stdin); //read the message in stdin
}


int main(int argc,char** argv){

  char msg_cli[50000];
  char msg_ser[50000];
  char end[] = "End of message\n";

  memset(msg_cli,0,sizeof(msg_cli));
  memset(msg_ser,0,sizeof(msg_ser));
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

  while (strcmp(msg_cli,end) !=0){
    if (connect == -1){
      printf("No connection \n");
      break;
    }
    handle_client_message(s,msg_cli);
    readline(s,msg_ser,sizeof(msg_cli));
    //Memory libeation
    memset(msg_ser, 0, sizeof(msg_ser));
  }

  //close Connection
  close(s);



  return 0;


}
